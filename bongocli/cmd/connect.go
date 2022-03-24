// Package cmd represents all CLI commands.
/*
Copyright © 2022 iu7og

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
package cmd

import (
	"errors"
	"fmt"
	"strings"

	"github.com/manifoldco/promptui"
	"github.com/spf13/cobra"
)

const (
	oneArg    = 1
	twoArgs   = 2
	threeArgs = 3
)

// connectCmd represents the connect command.
var connectCmd = &cobra.Command{
	Use:   "connect",
	Short: "Connect to bongodb server instance",
	Long: `Connect to existent bongodb instance.

Pass server address for connection. Address should be in host:port format.
For example:

bongocli connect localhost:5000`,
	Args: cobra.MinimumNArgs(1),
	RunE: execConnect,
}

func init() {
	rootCmd.AddCommand(connectCmd)
}

func execConnect(cmd *cobra.Command, args []string) error {
	//c, err := connect.Connect(args[0])
	//if err != nil {
	//	return err
	//}

	prompt := promptui.Prompt{
		Label:     ">",
		Validate:  promptValidate,
		Templates: promptTemplates(),
	}

	for {
		res, err := prompt.Run()
		if err != nil {
			return err
		}

		resParts := strings.Split(res, " ")
		switch resParts[0] {
		case "exit":
			fmt.Println("Exiting")

			return nil
		default:
			fmt.Printf("Got command: '%s'\n", res)
		}
	}
}

func promptValidate(input string) error {
	cmdParts := strings.Split(input, " ")

	zeroArgsCmds := map[string]struct{}{
		"truncate": {},
		"exit":     {},
	}
	oneArgCmds := map[string]struct{}{
		"get":    {},
		"delete": {},
	}
	twoArgsCmds := map[string]struct{}{
		"set": {},
	}

	switch len(cmdParts) {
	case oneArg:
		if _, ok := zeroArgsCmds[strings.ToLower(cmdParts[0])]; !ok {
			return errors.New("invalid command")
		}
	case twoArgs:
		if _, ok := oneArgCmds[strings.ToLower(cmdParts[0])]; !ok {
			return errors.New("invalid command")
		}
	case threeArgs:
		if _, ok := twoArgsCmds[strings.ToLower(cmdParts[0])]; !ok {
			return errors.New("invalid command")
		}
	default:
		return errors.New("invalid command or to much arguments for command call")
	}

	return nil
}

func promptTemplates() *promptui.PromptTemplates {
	return &promptui.PromptTemplates{
		Prompt:  "{{ . }} ",
		Valid:   "{{ . | green }} ",
		Invalid: "{{ . | red }} ",
		Success: "{{ . | bold }} ",
	}
}
