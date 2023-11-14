package server

import (
	"Havoc/pkg/logger"
	"github.com/gorilla/websocket"
)

const (
	UserStatusOffline = 0
	UserStatusOnline  = 1
)

func (t *Teamserver) UserAuthenticate(username, password string) bool {
	if t.Profile.Config.Operators != nil {
		for _, User := range t.Profile.Config.Operators.Users {
			if User.Name == username && User.Password == password {
				return true
			}
		}
	}

	return false
}

func (t *Teamserver) UserLogin(token string, login any, socket *websocket.Conn) {
	var (
		username string
	)

	switch login.(type) {
	case map[string]any:

		switch login.(map[string]any)["username"].(type) {
		case string:
			username = login.(map[string]any)["username"].(string)
			break
		default:
			logger.DebugError("UserLogin: invalid login username type")
			return
		}

		break

	default:
		logger.DebugError("UserLogin: invalid login type")
		return
	}

	// store the token and client data & socket into the sync map
	t.clients.Store(token, client{
		socket:   socket,
		username: username,
	})
}

func (t *Teamserver) UserStatus(username string) int {
	var status = UserStatusOffline

	t.clients.Range(func(key, value any) bool {
		if value.(client).username == username {
			status = UserStatusOnline
			return false
		}
		return true
	})

	return status
}
