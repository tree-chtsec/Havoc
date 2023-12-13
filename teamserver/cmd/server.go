package cmd

import (
	"os"
	"time"

	"Havoc/cmd/server"
	"Havoc/pkg/colors"
	"Havoc/pkg/logger"

	"github.com/spf13/cobra"
)

var CobraServer = &cobra.Command{
	Use:          "server",
	Short:        "teamserver command",
	SilenceUsage: true,
	RunE: func(cmd *cobra.Command, args []string) error {
		var (
			DirPath, _  = os.Getwd()
			ServerTimer = time.Now()
			Server      *server.Teamserver
		)

		if len(os.Args) <= 2 {
			err := cmd.Help()
			if err != nil {
				return err
			}
			os.Exit(0)
		}

		if Server = server.NewTeamserver(); Server == nil {
			logger.Error("failed to create server")
			return nil
		}

		Server.SetServerFlags(flags)

		startMenu()

		if flags.Server.Debug {
			logger.SetDebug(true)
			logger.Debug("Debug mode enabled")
		}

		logger.Info("%v [Version: %v %v]", colors.BoldWhite("Havoc Framework"), server.Version, server.CodeName)

		if flags.Server.Default {
			Server.SetProfile(DirPath + "/data/havoc.yaotl")
		} else if flags.Server.Profile != "" {
			Server.SetProfile(flags.Server.Profile)
		} else {
			logger.Error("No profile specified. Specify a profile with --profile or choose the standard profile with --default")
			os.Exit(1)
		}

		logger.Info("Time: " + colors.Yellow(ServerTimer.Format("02/01/2006 15:04:05")))

		Server.Start()

		os.Exit(0)

		return nil
	},
}
