package server

import (
	"Havoc/pkg/logger"
	"encoding/json"
	"errors"
)

func (t *Teamserver) AgentRegister(name string, agent map[string]any) error {
	var (
		payload Handler
		data    []byte
		err     error
	)

	payload = Handler{
		Name: name,
		Data: agent,
	}

	// add the payload to the
	// available payloads lists
	t.payloads = append(t.payloads, payload)

	// convert the object to a json
	if data, err = json.Marshal(payload); err != nil {
		return err
	}

	// broadcast to all connected clients
	// and any future clients
	t.UserBroadcast(true, t.EventCreateJson(EventAgentRegister, data))

	return nil
}

func (t *Teamserver) AgentProcess(context map[string]any, request []byte) ([]byte, error) {
	return nil, nil
}

func (t *Teamserver) AgentGenerate(ctx map[string]any, config map[string]any) ([]byte, error) {
	for _, agent := range t.payloads {

		logger.Debug("%v == %v", agent.Name, ctx["name"].(string))

		if agent.Data["name"].(string) == ctx["name"].(string) {
			return t.plugins.AgentGenerate(ctx, config)
		}
	}

	return nil, errors.New("agent to generate not found")
}

func (t *Teamserver) AgentBuildLog(ctx any, fmt string, args ...any) {

}
