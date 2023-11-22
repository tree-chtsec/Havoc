package server

import "time"

const (
	EventUserLogin   = "user::login"
	EventUserLogout  = "user::logout"
	EventUserMessage = "user::message"
)

const (
	EventListenerRegister = "listener::register"
	EventListenerStart    = "listener::start"
	EventListenerEdit     = "listener::edit"
	EventListenerStop     = "listener::stop"
)

const (
	EventAgentInitialize = "agent::register"
	EventAgentCallback   = "agent::console"
	EventAgentRemove     = "agent::remove"
	EventAgentInput      = "agent::input"
	EventAgentStatus     = "agent::status"
)

func (t *Teamserver) EventCreate(event string, data any) map[string]any {
	return map[string]any{
		"time": time.Now().Format("02/01/2006 15:04:05"),
		"type": event,
		"data": data,
	}
}
