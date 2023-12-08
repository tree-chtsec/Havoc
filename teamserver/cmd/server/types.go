package server

import (
	"Havoc/pkg/agent"
	"Havoc/pkg/db"
	"Havoc/pkg/packager"
	"Havoc/pkg/profile"
	"Havoc/pkg/service"
	"Havoc/pkg/webhook"
	"sync"

	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"
)

type Listener struct {
	Name   string
	Type   int
	Config any
}

type Client struct {
	ClientID      string
	Username      string
	GlobalIP      string
	ClientVersion string
	Connection    *websocket.Conn
	Packager      *packager.Packager
	Authenticated bool
	SessionID     string
	Mutex         sync.Mutex
}

type HavocUser struct {
	username string
	socket   *websocket.Conn
}

type HavocAgent struct {
	// uuid this is the identifier
	// for the teamserver to know
	uuid string

	// context member contains the context of the agent
	// like to what listener it is associated with and
	// other things that is important to the teamserver
	// to know, like: parent agent, listener name, etc.
	context map[string]any

	// metadata contains the metadata of the agent
	// like hostname, username, process name and id, etc.
	metadata map[string]any
}

type Users struct {
	Name     string
	Password string
	Hashed   bool
	Online   bool
}

type serverFlags struct {
	Host string
	Port string

	Profile  string
	Verbose  bool
	Debug    bool
	DebugDev bool
	SendLogs bool
	Default  bool
}

type utilFlags struct {
	NoBanner bool
	Debug    bool
	Verbose  bool

	Test bool

	ListOperators bool
}

type TeamserverFlags struct {
	Server serverFlags
	Util   utilFlags
}

type Endpoint struct {
	Endpoint string
	Function func(ctx *gin.Context)
}

type Handler struct {
	Name string         `json:"name"`
	Data map[string]any `json:"data"`
}

type Teamserver struct {
	Flags      TeamserverFlags
	Profile    *profile.Profile
	Clients    sync.Map // map[string]*Client
	Users      []Users
	EventsList []packager.Package
	Service    *service.Service
	WebHooks   *webhook.WebHook
	DB         *db.DB

	Server *ServerApi

	Agents    agent.Agents
	Endpoints []*Endpoint

	Settings struct {
		Compiler64 string
		Compiler32 string
		Nasm       string
	}

	configPath string
	events     struct {
		mutex sync.Mutex
		list  []map[string]any
	}

	clients   sync.Map
	plugins   *PluginSystem
	protocols []Handler // available handlers/listeners to use
	listener  []Handler // started listeners
	agents    sync.Map  // current connected agents
}
