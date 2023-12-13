package server

import (
	"encoding/json"
	"errors"
	"fmt"
)

// ListenerRegister
// register a listener to the server and
// notify clients to a newly available handler.
func (t *Teamserver) ListenerRegister(name string, listener map[string]any) error {
	var (
		protocol Handler
		data     []byte
		err      error
	)

	protocol = Handler{
		Name: name,
		Data: listener,
	}

	// add the listener to the
	// available protocols lists
	t.protocols = append(t.protocols, protocol)

	// convert the object to a json
	if data, err = json.Marshal(protocol); err != nil {
		return err
	}

	// broadcast to all connected clients
	// and any future clients
	t.UserBroadcast(true, t.EventCreateJson(EventListenerRegister, data))

	return nil
}

func (t *Teamserver) ListenerStart(name, protocol string, options map[string]any) error {
	var (
		err    error
		data   map[string]string
		host   string
		port   string
		status string
	)

	for _, prot := range t.protocols {
		if val, ok := prot.Data["protocol"]; ok {
			if val.(string) == protocol {
				if data, err = t.plugins.ListenerStart(name, protocol, options); err != nil {
					return err
				}

				host, ok = data["host"]
				port, ok = data["port"]
				status, ok = data["status"]

				t.UserBroadcast(true, t.EventCreate(EventListenerStart, map[string]any{
					"name":     name,
					"protocol": protocol,
					"host":     host,
					"port":     port,
					"status":   status,
				}))

				break
			}
		}
	}

	return nil
}

func (t *Teamserver) ListenerEvent(protocol string, event map[string]any) (map[string]any, error) {
	var (
		err  error
		resp map[string]any
	)

	err = errors.New("protocol not found")

	for _, p := range t.protocols {
		if val, ok := p.Data["protocol"]; ok {
			if val.(string) == protocol {
				if resp, err = t.plugins.ListenerEvent(protocol, event); err != nil {
					return nil, err
				}

				break
			}
		}
	}

	return resp, err
}

func (t *Teamserver) ListenerLog(name string, format string, args ...any) {
	t.UserBroadcast(true, t.EventCreate(EventListenerLog, map[string]any{
		"name": name,
		"log":  fmt.Sprintf(format, args...),
	}))
}
