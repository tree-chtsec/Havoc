package server

import "C"
import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"github.com/gorilla/websocket"
	"os"
	"os/exec"
	"strconv"
	"strings"

	"Havoc/pkg/agent"
	"Havoc/pkg/colors"
	"Havoc/pkg/db"
	"Havoc/pkg/events"
	"Havoc/pkg/logger"
	"Havoc/pkg/packager"
	"Havoc/pkg/profile"
)

var (
	Version  = "0.8"
	CodeName = "Killer Queen"
)

func NewTeamserver(DatabasePath string) *Teamserver {
	var (
		server   = new(Teamserver)
		err      error
		database *db.DB
	)

	if database, err = db.DatabaseNew(DatabasePath); err != nil {
		logger.Error("Failed to create a new db: " + err.Error())
		return nil
	}

	server.DB = database

	if err = server.createConfigPath(); err != nil {
		logger.Error("couldn't create config folder: %v", err.Error())
		return nil
	}

	return server
}

func (t *Teamserver) SetServerFlags(flags TeamserverFlags) {
	t.Flags = flags
}

func (t *Teamserver) Start() {
	var (
		ServerFinished chan bool
		err            error
	)

	if t.Flags.Server.Host == "" {
		t.Flags.Server.Host = t.Profile.ServerHost()
	}

	if t.Flags.Server.Port == "" {
		t.Flags.Server.Port = strconv.Itoa(t.Profile.ServerPort())
	}

	if t.Server, err = NewServerApi(t); err != nil {
		logger.Error("Failed to start api server: " + err.Error())
		return
	}

	// generate a new plugin system instance
	t.plugins = NewPluginSystem(t)
	if err = t.plugins.RegisterPlugin("../HavocPlugins/http.hp/http.hp"); err != nil {
		logger.Error("failed to load plugin: %v", err)
	}

	// start the api server
	go t.Server.Start(t.Flags.Server.Host, t.Flags.Server.Port, t.ConfigPath(), &ServerFinished)

	logger.Info("Starting Teamserver on %v", colors.BlueUnderline("https://"+t.Flags.Server.Host+":"+t.Flags.Server.Port))

	// This should hold the Teamserver as long as the WebSocket Server is running
	logger.Debug("Wait til the server shutdown")

	<-ServerFinished
}

// Version
// get the current server version
func (*Teamserver) Version() map[string]string {
	return map[string]string{
		"version":  Version,
		"codename": CodeName,
	}
}

func (t *Teamserver) createConfigPath() error {
	var err error

	if t.configPath, err = os.UserHomeDir(); err != nil {
		return err
	}

	t.configPath += "/.havoc"

	if err = os.MkdirAll(t.configPath, os.ModePerm); err != nil {
		return err
	}

	return nil
}

func (t *Teamserver) ConfigPath() string {
	return t.configPath
}

func (t *Teamserver) SetProfile(path string) {
	t.Profile = profile.NewProfile()
	logger.LoggerInstance.STDERR = os.Stderr
	err := t.Profile.SetProfile(path, t.Flags.Server.Default)
	if err != nil {
		logger.SetStdOut(os.Stderr)
		logger.Error("Profile error:", colors.Red(err))
		os.Exit(1)
	}
}

func (t *Teamserver) EventBroadcast(ExceptClient string, pk packager.Package) {

	// some sanity check
	if pk.Head.Event == 0 {
		return
	}

	t.Clients.Range(func(key, value any) bool {
		ClientID := key.(string)
		if ExceptClient != ClientID {
			err := t.SendEvent(ClientID, pk)
			if err != nil && !strings.Contains(err.Error(), "use of closed network connection") {
				logger.Error("SendEvent error: ", colors.Red(err))
			}
		}
		return true
	})
}

func (t *Teamserver) EventNewDemon(DemonAgent *agent.Agent) packager.Package {
	return events.Demons.NewDemon(DemonAgent)
}

func (t *Teamserver) EventAgentMark(AgentID, Mark string) {
	var pk = events.Demons.MarkAs(AgentID, Mark)

	t.EventAppend(pk)
	t.EventBroadcast("", pk)
}

func (t *Teamserver) EventListenerError(ListenerName string, Error error) {
	var pk = events.Listener.ListenerError("", ListenerName, Error)

	t.EventAppend(pk)
	t.EventBroadcast("", pk)

	// also remove the listener from the init packages.
	for EventID := range t.EventsList {
		if t.EventsList[EventID].Head.Event == packager.Type.Listener.Type {
			if t.EventsList[EventID].Body.SubEvent == packager.Type.Listener.Add {
				if name, ok := t.EventsList[EventID].Body.Info["Name"]; ok {
					if name == ListenerName {
						t.EventsList[EventID].Body.Info["Status"] = "Offline"
						t.EventsList[EventID].Body.Info["Error"] = Error.Error()
					}
				}
			}
		}
	}
}

func (t *Teamserver) SendEvent(id string, pk packager.Package) error {
	var (
		buffer bytes.Buffer
		err    error
	)

	err = json.NewEncoder(&buffer).Encode(pk)
	if err != nil {
		return err
	}

	value, isOk := t.Clients.Load(id)
	if isOk {
		client := value.(*Client)
		client.Mutex.Lock()

		err = client.Connection.WriteMessage(websocket.BinaryMessage, buffer.Bytes())
		if err != nil {
			// TODO: comment this line out as it seems to crash the server
			//t.Clients[id].Mutex.Unlock()
			return err
		}

		client.Mutex.Unlock()

	} else {
		return errors.New(fmt.Sprintf("client (%v) doesn't exist anymore", colors.Red(id)))
	}

	return nil
}

func (t *Teamserver) RemoveClient(ClientID string) {

	value, isOk := t.Clients.Load(ClientID)

	if isOk {
		client := value.(*Client)
		var (
			userDisconnected = client.Username
			Authenticated    = client.Authenticated
		)

		if Authenticated {
			t.EventBroadcast(ClientID, events.ChatLog.UserDisconnected(userDisconnected))
			for UserID := range t.Users {
				if userDisconnected == t.Users[UserID].Name {
					t.Users[UserID].Online = false
				}
			}
		}

		t.Clients.Delete(ClientID)
	}
}

func (t *Teamserver) EventAppend(event packager.Package) []packager.Package {
	// some sanity check
	if event.Head.Event == 0 {
		return t.EventsList
	}

	if event.Head.OneTime != "true" {
		t.EventsList = append(t.EventsList, event)
		return append(t.EventsList, event)
	}

	return nil
}

func (t *Teamserver) EventRemove(EventID int) []packager.Package {
	t.EventsList = append(t.EventsList[:EventID], t.EventsList[EventID+1:]...)

	return append(t.EventsList[:EventID], t.EventsList[EventID+1:]...)
}

func (t *Teamserver) SendAllPackagesToNewClient(ClientID string) {
	for _, Package := range t.EventsList {
		err := t.SendEvent(ClientID, Package)
		if err != nil {
			logger.Error("error while sending info to client("+ClientID+"): ", err)
			return
		}
	}

	// send all the agents that are alive right now to the new client
	for _, demon := range t.Agents.Agents {
		if demon.Active == false {
			continue
		}

		pk := t.EventNewDemon(demon)
		err := t.SendEvent(ClientID, pk)
		if err != nil {
			logger.Error("error while sending info to client("+ClientID+"): ", err)
			return
		}
	}
}

func (t *Teamserver) FindSystemPackages() bool {
	var err error

	if t.Profile.Config.Server.Build != nil {

		if len(t.Profile.Config.Server.Build.Compiler64) > 0 {
			if _, err := os.Stat(t.Profile.Config.Server.Build.Compiler64); os.IsNotExist(err) {
				logger.SetStdOut(os.Stderr)
				logger.Error("Compiler x64 path doesn't exist: " + t.Profile.Config.Server.Build.Compiler64)
				return false
			}

			t.Settings.Compiler64 = t.Profile.Config.Server.Build.Compiler64
		} else {
			t.Settings.Compiler64, err = exec.LookPath("x86_64-w64-mingw32-gcc")
			if err != nil {
				logger.SetStdOut(os.Stderr)
				logger.Error("Couldn't find x64 mingw compiler: " + err.Error())
				return false
			}
		}

		if len(t.Profile.Config.Server.Build.Compiler86) > 0 {
			if _, err := os.Stat(t.Profile.Config.Server.Build.Compiler86); os.IsNotExist(err) {
				logger.SetStdOut(os.Stderr)
				logger.Error("Compiler x86 path doesn't exist: " + t.Profile.Config.Server.Build.Compiler86)
				return false
			}

			t.Settings.Compiler32 = t.Profile.Config.Server.Build.Compiler86
		} else {
			t.Settings.Compiler32, err = exec.LookPath("i686-w64-mingw32-gcc")
			if err != nil {
				logger.SetStdOut(os.Stderr)
				logger.Error("Couldn't find x86 mingw compiler: " + err.Error())
				return false
			}
		}

		if len(t.Profile.Config.Server.Build.Nasm) > 0 {
			if _, err := os.Stat(t.Profile.Config.Server.Build.Nasm); os.IsNotExist(err) {
				logger.SetStdOut(os.Stderr)
				logger.Error("Nasm path doesn't exist: " + t.Profile.Config.Server.Build.Nasm)
				return false
			}

			t.Settings.Nasm = t.Profile.Config.Server.Build.Nasm
		} else {
			t.Settings.Nasm, err = exec.LookPath("nasm")
			if err != nil {
				logger.Error("Couldn't find nasm: " + err.Error())
				return false
			}
		}

	} else {
		t.Settings.Compiler64, err = exec.LookPath("x86_64-w64-mingw32-gcc")
		if err != nil {
			logger.SetStdOut(os.Stderr)
			logger.Error("Couldn't find x64 mingw compiler: " + err.Error())
			return false
		}

		t.Settings.Compiler32, err = exec.LookPath("i686-w64-mingw32-gcc")
		if err != nil {
			logger.SetStdOut(os.Stderr)
			logger.Error("Couldn't find x86 mingw compiler: " + err.Error())
			return false
		}

		t.Settings.Nasm, err = exec.LookPath("nasm")
		if err != nil {
			logger.SetStdOut(os.Stderr)
			logger.Error("Couldn't find nasm: " + err.Error())
			return false
		}
	}

	logger.Info(fmt.Sprintf(
		"Build: \n"+
			" - Compiler x64 : %v\n"+
			" - Compiler x86 : %v\n"+
			" - Nasm         : %v",
		colors.Blue(t.Settings.Compiler64),
		colors.Blue(t.Settings.Compiler32),
		colors.Blue(t.Settings.Nasm),
	))

	return true
}

func (t *Teamserver) EndpointAdd(endpoint *Endpoint) bool {
	for _, e := range t.Endpoints {
		if e.Endpoint == endpoint.Endpoint {
			return false
		}
	}

	t.Endpoints = append(t.Endpoints, endpoint)

	return true
}

func (t *Teamserver) EndpointRemove(endpoint string) []*Endpoint {
	for i := range t.Endpoints {
		if t.Endpoints[i].Endpoint == endpoint {
			t.Endpoints = append(t.Endpoints[:i], t.Endpoints[i+1:]...)
			return t.Endpoints
		}
	}

	return t.Endpoints
}
