package server

import (
	"Havoc/pkg/logger"
	"Havoc/pkg/utils"
	"errors"
	"fmt"
	"plugin"
	"sync"
)

const (
	PluginTypeListener   = "listener"
	PluginTypeAgent      = "agent"
	PluginTypeManagement = "management"
)

type HavocListenerCtx struct {
	name   string
	handle any
}

type BasicInterface interface {
	Register() map[string]any
}

type ListenerInterface interface {
	ListenerStart(config map[string]any) (map[string]any, error)
	ListenerEdit(config map[string]any) (map[string]any, error)
	ListenerStop(name string) error
}

type AgentInterface interface{}
type ManagementInterface interface{}

type Plugin struct {
	Name string
	Type string
	Data map[string]any

	// the loaded plugin interface that are callable
	BasicInterface
	ListenerInterface
	AgentInterface
	ManagementInterface
}

type PluginSystem struct {
	// loaded havoc plugins
	// those plugins have been
	// registered and are safe to use
	loaded sync.Map
}

// NewPluginSystem
// create a new plugin system instance
func NewPluginSystem() *PluginSystem {
	return new(PluginSystem)
}

// RegisterPlugin is going to register a specified havoc plugin
func (s *PluginSystem) RegisterPlugin(path string) error {
	var (
		err      error
		open     *plugin.Plugin
		lookup   plugin.Symbol
		inter    BasicInterface
		register map[string]any
	)

	// try to open plugin
	if open, err = plugin.Open(path); err != nil {
		return err
	}

	// try to get the exported plugin object
	// to register the plugin and retrieve metadata about it
	if lookup, err = open.Lookup("Plugin"); err != nil {
		return err
	}

	// cast the looked up symbol to
	// the HavocPlugin interface
	inter = lookup.(BasicInterface)

	// try to register the plugin
	register = inter.Register()

	logger.Debug("register: %v\n", register)

	// add plugin to the internal sync
	// map and make it available
	if err = s.AddPlugin(register, lookup); err != nil {
		return fmt.Errorf("failed to register plugin: %v", err.Error())
	}

	return err
}

// AddPlugin the registered plugin to see if there
// wasn't given any faulty or lacking info and
// creates a havoc Plugin object
func (s *PluginSystem) AddPlugin(register map[string]any, inter any) error {
	var (
		extension = new(Plugin)
	)

	if len(register) == 0 {
		return errors.New("register is empty")
	}

	// get the name of the registered plugin
	switch register["name"].(type) {
	case string:

		// check if the name is empty.
		// if yes then return an error
		if len(register["name"].(string)) == 0 {
			return errors.New("register.name is empty")
		}

		// set the name of the extension/plugin
		extension.Name = register["name"].(string)

		break

	default:
		return errors.New("register.name is invalid type: excepted string")
	}

	// get the type of the registered plugin
	switch register["type"].(type) {
	case string:
		switch register["type"].(string) {
		case PluginTypeListener:
			extension.ListenerInterface = inter.(ListenerInterface)

			a, e := extension.ListenerStart(map[string]any{})

			logger.Debug("a: %v -> e: %v\n", a, e)

			break
		case PluginTypeAgent:
			extension.AgentInterface = inter.(AgentInterface)
			break
		case PluginTypeManagement:
			extension.ManagementInterface = inter.(ManagementInterface)
			break
		default:
			return errors.New("invalid register type")
		}

		extension.Type = register["type"].(string)

		break

	default:
		return errors.New("register.type is invalid type: excepted string")
	}

	// add the extension to the sync map
	s.loaded.Store(utils.GenerateID(32), extension)

	return nil
}
