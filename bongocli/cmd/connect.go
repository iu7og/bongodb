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
	"context"
	"encoding/csv"
	"errors"
	"fmt"
	"strings"

	"github.com/iu7og/bongodb/bongocli/internal/connect"
	"github.com/iu7og/bongodb/bongocli/internal/pb/bongodb"
	"github.com/manifoldco/promptui"
	"github.com/spf13/cobra"
	"google.golang.org/protobuf/types/known/emptypb"
	"google.golang.org/protobuf/types/known/wrapperspb"
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

// execConnect is a handler for connect CLI command.
func execConnect(_ *cobra.Command, args []string) error {
	var (
		err         error
		c           bongodb.BongoDBClient
		cmd         string
		cmdPartsRaw []string
	)

	c, err = connect.Connect(args[0])
	if err != nil {
		return err
	}

	prompt := promptui.Prompt{
		Label:     ">",
		Validate:  promptValidate,
		Templates: promptTemplates(),
	}

	ctx := context.Background()
	for {
		cmd, err = prompt.Run()
		if err != nil {
			return err
		}

		r := csv.NewReader(strings.NewReader(cmd))
		r.Comma = ' '
		cmdPartsRaw, err = r.Read()
		if err != nil {
			return err
		}

		cmdParts := make([]string, 0, len(cmdPartsRaw))
		for _, part := range cmdPartsRaw {
			if part != "" {
				cmdParts = append(cmdParts, part)
			}
		}

		switch cmdParts[0] {
		case "exit":
			return nil
		case "set":
			_, err = c.Set(ctx, &bongodb.KeyValueRequest{
				Key:   &wrapperspb.StringValue{Value: cmdParts[1]},
				Value: &wrapperspb.StringValue{Value: cmdParts[2]},
			})
			if err != nil {
				fmt.Printf("Error while executing set, error is: %s\n", err)
			}
		case "get":
			var res *bongodb.ValueResponse
			res, err = c.Get(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: cmdParts[1]}})
			if err != nil {
				fmt.Printf("Error while executing get, error is: %s\n", err)
			} else {
				fmt.Println(res.GetValue().GetValue())
			}
		case "delete":
			_, err = c.Delete(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: cmdParts[1]}})
			if err != nil {
				fmt.Printf("Error while executing delete, error is: %s\n", err)
			} else {
				fmt.Printf("Key '%s' is deleted\n", cmdParts[1])
			}
		case "truncate":
			_, err = c.Truncate(ctx, &emptypb.Empty{})
			if err != nil {
				fmt.Printf("Error while executing truncate, error is: %s\n", err)
			} else {
				fmt.Println("Store is cleared")
			}
		}
	}
}

// promptValidate is used for on-fly validation of input command.
func promptValidate(input string) error {
	r := csv.NewReader(strings.NewReader(input))
	r.Comma = ' '
	cmdPartsRaw, err := r.Read()
	if err != nil {
		return err
	}

	cmdParts := make([]string, 0, len(cmdPartsRaw))
	for _, part := range cmdPartsRaw {
		if part != "" {
			cmdParts = append(cmdParts, part)
		}
	}

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
		return errors.New("invalid command or too much arguments for command call")
	}

	return nil
}

// promptTemplates returns visualization template for prompt.
func promptTemplates() *promptui.PromptTemplates {
	return &promptui.PromptTemplates{
		Prompt:  "{{ . }} ",
		Valid:   "{{ . | green }} ",
		Invalid: "{{ . | red }} ",
		Success: "{{ . | bold }} ",
	}
}
