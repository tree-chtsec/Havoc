package server

import (
	"os"
	"strconv"

	"Havoc/pkg/api"
	"Havoc/pkg/colors"
	"Havoc/pkg/logger"
	"Havoc/pkg/plugin"
	"Havoc/pkg/profile"
)

var (
	Version  = "1.0"
	CodeName = "Killer Queen"
)

func NewTeamserver() *Teamserver {
	var (
		server = new(Teamserver)
		err    error
	)

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
		plugins        []string
	)

	plugins = []string{
		"../HavocPlugins/kaine-kit/http.hp/http.hp",
		"../HavocPlugins/kaine-kit/kaine.hp/kaine.hp",
	}

	if t.Flags.Server.Host == "" {
		t.Flags.Server.Host = t.Profile.ServerHost()
	}

	if t.Flags.Server.Port == "" {
		t.Flags.Server.Port = strconv.Itoa(t.Profile.ServerPort())
	}

	if t.Server, err = api.NewServerApi(t); err != nil {
		logger.Error("failed to start api server: " + err.Error())
		return
	}

	// generate a new plugin system instance
	t.plugins = plugin.NewPluginSystem(t)

	// load all plugins that has been specified in the folder
	logger.Info("loading plugins [%v]:", len(plugins))
	for i := range plugins {
		var ext *plugin.Plugin

		if ext, err = t.plugins.RegisterPlugin(plugins[i]); err != nil {
			logger.Error("failed to load plugin: %v", err)
		}

		if ext != nil {
			logger.Info(" %v plugin \"%v\" loaded", colors.Blue("*"), colors.Blue(ext.Name))
		}
	}

	// start the api server
	go t.Server.Start(t.Flags.Server.Host, t.Flags.Server.Port, t.ConfigPath(), &ServerFinished)

	logger.Info("starting server on %v", colors.BlueUnderline("https://"+t.Flags.Server.Host+":"+t.Flags.Server.Port))

	// This should hold the Teamserver as long as the WebSocket Server is running
	logger.Debug("wait til the server shutdown")

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

	t.configPath += "/.havoc/server"

	if err = os.MkdirAll(t.configPath, os.ModePerm); err != nil {
		return err
	}

	// create listeners directory
	if err = os.MkdirAll(t.configPath+"/listeners", os.ModePerm); err != nil {
		return err
	}

	// create agents directory
	if err = os.MkdirAll(t.configPath+"/agents", os.ModePerm); err != nil {
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
