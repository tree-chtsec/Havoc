package server

import (
	"Havoc/pkg/cert"
	"Havoc/pkg/logger"
	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"

	"crypto/rand"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"sync"
)

const (
	ApiMaxRequestRead = 50000000 // 50mb
	ApiTokenLength    = 32
)

type teamserver interface {
	UserAuthenticate(username, password string) bool
	UserLogin(token string, login any, socket *websocket.Conn)
	UserLogoutByToken(user string) error
	UserNameByToken(token string) (string, error)
	UserStatus(username string) int

	ListenerStart(name, protocol string, options map[string]any) error
	ListenerEvent(protocol string, event map[string]any) (map[string]any, error)
}

type ServerApi struct {
	Engine *gin.Engine

	// teamserver interface
	// to interact with some functions to
	// add/query/remove objects and data
	// to from the teamserver
	teamserver teamserver

	// wait queue for websockets to send
	// the access token to register the
	// event endpoint
	waitQueue sync.Map
}

func NewServerApi(teamserver teamserver) (*ServerApi, error) {
	var api = new(ServerApi)

	gin.SetMode(gin.ReleaseMode)

	api.Engine = gin.New()
	api.teamserver = teamserver

	// set api endpoints
	api.Engine.POST("/api/login", api.login)

	// listeners
	api.Engine.POST("/api/listener/start", api.listenerStart)
	api.Engine.POST("/api/listener/stop", api.listenerStop)
	api.Engine.POST("/api/listener/edit", api.listenerEdit)
	api.Engine.POST("/api/listener/event", api.listenerEvent)

	// websocket event endpoint
	api.Engine.GET("/api/event", api.event)

	return api, nil
}

// Start the server api
// generates an HTTP certificate and then starts the api server
func (api *ServerApi) Start(host, port, certsPath string, finished *chan bool) {
	var (
		certPath = certsPath + "/server.cert"
		keyPath  = certsPath + "/server.key"
		err      error
		Cert     []byte
		Key      []byte
	)

	// generate cert and key based on host
	Cert, Key, err = cert.HTTPSGenerateRSACertificate(host)
	if err != nil {
		logger.Error("Failed to generate server certificates: " + err.Error())
		os.Exit(0)
	}

	// write cert file to disk
	err = os.WriteFile(certPath, Cert, 0644)
	if err != nil {
		logger.Error("Couldn't save server cert file: " + err.Error())
		return
	}

	// write the cert key path to disk
	err = os.WriteFile(keyPath, Key, 0644)
	if err != nil {
		logger.Error("Couldn't save server cert file: " + err.Error())
		return
	}

	logger.Debug("certPath: %v", certPath)

	// start the api server
	if err = api.Engine.RunTLS(host+":"+port, certPath, keyPath); err != nil {
		logger.Error("Failed to start webserver: " + err.Error())
		return
	}

	// signal that we finished
	*finished <- true
}

// login endpoint for the client to log in
// and retrieve an access token
func (api *ServerApi) login(ctx *gin.Context) {
	var (
		body  []byte
		token string
		err   error
		user  string
		pass  string
		login map[string]any
	)

	// read from request the login data
	if body, err = io.ReadAll(io.LimitReader(ctx.Request.Body, ApiMaxRequestRead)); err != nil {
		logger.DebugError("Failed to read from server api login request: " + err.Error())
		goto ERROR
	}

	logger.Debug("got request on /api/login:" + fmt.Sprintf("%s", string(body)))

	// unmarshal the bytes into a map
	if err = json.Unmarshal(body, &login); err != nil {
		logger.DebugError("Failed to unmarshal bytes to map: " + err.Error())
		return
	}

	// generate a token
	// if failed then abort with status code
	// 500: Internal Server Error
	if token = api.generateToken(); len(token) == 0 {
		ctx.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	// get username from login request
	switch login["username"].(type) {
	case string:
		user = login["username"].(string)
	default:
		logger.DebugError("Failed retrieve username: invalid type")
		goto ERROR
	}

	// check if the specified user
	// is already active/online
	if api.teamserver.UserStatus(user) == UserStatusOnline {
		ctx.JSON(http.StatusUnauthorized, gin.H{
			"error": "user is already online",
		})
		return
	}

	// get password from login request
	switch login["password"].(type) {
	case string:
		pass = login["password"].(string)
	default:
		logger.DebugError("Failed retrieve password: invalid type")
		goto ERROR
	}

	// check if we can authenticate the user
	if !api.teamserver.UserAuthenticate(user, pass) {
		ctx.AbortWithStatus(http.StatusUnauthorized)
		return
	}

	// send json token response
	ctx.JSON(http.StatusOK, gin.H{
		"token": token,
	})

	// store the token inside the wait queue
	api.waitQueue.Store(token, login)

	return

ERROR:
	ctx.AbortWithStatus(http.StatusInternalServerError)
}

// event is a websocket endpoint that
// allows a client to listen for incoming
// events (registered agent, operator connect, etc.)
func (api *ServerApi) event(ctx *gin.Context) {
	var (
		upgrade websocket.Upgrader
		socket  *websocket.Conn
		err     error
	)

	// upgrade this request to a websocket
	if socket, err = upgrade.Upgrade(ctx.Writer, ctx.Request, nil); err != nil {
		logger.DebugError("Failed upgrading request: " + err.Error())
		return
	}

	// handle socket channel.
	// check if the token is available
	go api.handleEventClient(socket)
}

func (api *ServerApi) listenerStart(ctx *gin.Context) {
	var (
		body     []byte
		err      error
		listener map[string]any
		name     string
		protocol string
		options  map[string]any
	)

	// read from request the login data
	if body, err = io.ReadAll(io.LimitReader(ctx.Request.Body, ApiMaxRequestRead)); err != nil {
		logger.DebugError("Failed to read from server api login request: " + err.Error())
		goto ERROR
	}

	logger.Debug("got request on /api/listener/start:" + fmt.Sprintf("%s", string(body)))

	// unmarshal the bytes into a map
	if err = json.Unmarshal(body, &listener); err != nil {
		logger.DebugError("Failed to unmarshal bytes to map: " + err.Error())
		return
	}

	// get name from listener start request
	switch listener["name"].(type) {
	case string:
		name = listener["name"].(string)
	default:
		logger.DebugError("Failed retrieve name: invalid type")
		goto ERROR
	}

	// get protocol from listener start request
	switch listener["protocol"].(type) {
	case string:
		protocol = listener["protocol"].(string)
	default:
		logger.DebugError("Failed retrieve protocol: invalid type")
		goto ERROR
	}

	// get options from listener start request
	switch listener["data"].(type) {
	case map[string]any:
		options = listener["data"].(map[string]any)
	default:
		logger.DebugError("Failed retrieve data: invalid type")
		goto ERROR
	}

	if err = api.teamserver.ListenerStart(name, protocol, options); err != nil {
		ctx.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}

	ctx.AbortWithStatus(http.StatusOK)
	return

ERROR:
	ctx.AbortWithStatus(http.StatusInternalServerError)
}

func (api *ServerApi) listenerStop(ctx *gin.Context) {

}

func (api *ServerApi) listenerEdit(ctx *gin.Context) {

}

func (api *ServerApi) listenerEvent(ctx *gin.Context) {
	var (
		body     []byte
		err      error
		event    map[string]any
		protocol string
	)

	// read from request the event data
	if body, err = io.ReadAll(io.LimitReader(ctx.Request.Body, ApiMaxRequestRead)); err != nil {
		logger.DebugError("Failed to read from server api login request: " + err.Error())
		goto ERROR
	}

	logger.Debug("got request on /api/listener/event:" + fmt.Sprintf("%s", string(body)))

	// unmarshal the bytes into a map
	if err = json.Unmarshal(body, &event); err != nil {
		logger.DebugError("Failed to unmarshal bytes to map: " + err.Error())
		return
	}

	// get protocol from listener start request
	switch event["protocol"].(type) {
	case string:
		protocol = event["protocol"].(string)
	default:
		logger.DebugError("Failed retrieve protocol: invalid type")
		goto ERROR
	}

	// process listener event
	if event, err = api.teamserver.ListenerEvent(protocol, event); err != nil {
		ctx.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}

	ctx.JSON(http.StatusOK, event)
	return

ERROR:
	ctx.AbortWithStatus(http.StatusInternalServerError)
}

// handleEventClient
// handles incoming event clients
//
// it is going to read the given token from the client and checks
// if it in the wait queue. After log in the token is going to be
// stored inside the wait queue til the client connects to the websocket
// endpoint and send the token it previously got from the /api/login
// endpoint. After all of this the teamserver is going to register
// the client as officially logged in.
//
//	socket *websocket.Conn
func (api *ServerApi) handleEventClient(socket *websocket.Conn) {
	var (
		body  []byte
		err   error
		data  map[string]any
		login any
		token string
		ok    bool
	)

	if socket == nil {
		return
	}

	// try to read the token from socket
	if _, body, err = socket.ReadMessage(); err != nil {
		logger.DebugError("failed reading message from socket: " + err.Error())
		goto ERROR
	}

	if len(body) < ApiTokenLength {
		logger.DebugError("invalid socket request. closing connection")
		goto ERROR
	}

	// unmarshal the body to a map
	if err = json.Unmarshal(body, &data); err != nil {
		logger.DebugError("failed unmarshal message from socket: " + err.Error())
		goto ERROR
	}

	// get token from request
	switch data["token"].(type) {
	case string:
		token = data["token"].(string)
	default:
		logger.DebugError("failed retrieve token: invalid type")
		goto ERROR
	}

	// check if is inside the wait queue
	if login, ok = api.waitQueue.Load(token); !ok {
		logger.DebugError("failed retrieve token from map: token entry not found")
		goto ERROR
	}

	// by now the client was successfully authenticated
	// we're going to register the user to the teamserver
	api.teamserver.UserLogin(token, login, socket)

	// delete the token from the wait queue
	api.waitQueue.Delete(token)

	// this loop is just there to check the state of the connection
	// and if the client somehow disconnected from the connection.
	// if the user disconnected then handle it by close the resources
	// related to that user session
	for {
		// check if any error arrived from reading any message or
		// checking the state of the connection
		if _, _, err = socket.ReadMessage(); err == nil {
			continue
		}

		logger.Debug("user connection closed [token: %v] -> %v", token, err)

		// log out the connected user client and send
		// a broadcast event to every connected client
		if err = api.teamserver.UserLogoutByToken(token); err != nil {
			logger.DebugError("failed to logout user by token: %v", err)
		}

		break
	}

ERROR:
	socket.Close()
}

// generateToken
// generate a token for the client to use for authenticate
// and to use to interact with the api with
func (api *ServerApi) generateToken() string {
	b := make([]byte, ApiTokenLength/2)
	if _, err := rand.Read(b); err != nil {
		return ""
	}
	return hex.EncodeToString(b)
}
