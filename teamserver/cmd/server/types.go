package server

import (
	"Havoc/pkg/profile"

	"sync"

	"github.com/gorilla/websocket"
)

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

type Handler struct {
	Name string         `json:"name"`
	Data map[string]any `json:"data"`
}

type Teamserver struct {
	Flags   TeamserverFlags
	Profile *profile.Profile

	Server *ServerApi

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
