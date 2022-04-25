// Package cmd represents all CLI commands.
package cmd

import (
	"encoding/csv"
	"errors"
	"fmt"
	"io"
	"strings"

	"github.com/iu7og/bongodb/bongocli/internal/bongodb"
	"github.com/manifoldco/promptui"
	"github.com/spf13/cobra"
)

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

// execConnect is a handler for 'connect' CLI command.
func execConnect(_ *cobra.Command, args []string) error {
	var (
		err    error
		client *bongodb.Client
		rawCmd string
		cmd    []string
	)

	client = bongodb.New(args[0])

	prompt := promptui.Prompt{
		Label:     ">",
		Validate:  validateCommand,
		Templates: getPromptTemplates(),
	}

	fmt.Println("Use Ctrl+C to exit")

	for {
		rawCmd, err = prompt.Run()
		if err != nil {
			if err == promptui.ErrInterrupt || err == promptui.ErrEOF {
				fmt.Println("Exiting")

				return nil
			}

			return fmt.Errorf("error while running prompt command in connect command: %w", err)
		}

		cmd, err = prepareCommand(rawCmd)
		if err != nil {
			return fmt.Errorf("error while preparing prompt command in connect command: %w", err)
		}

		handleCommand(client, cmd)
	}
}

// validateCommand is used for on-fly validation of input command.
func validateCommand(rawCmd string) error {
	cmd, err := prepareCommand(rawCmd)
	if err != nil {
		if strings.HasSuffix(err.Error(), io.EOF.Error()) {
			return errors.New("command can't be empty")
		}

		return fmt.Errorf("error while preparing prompt command in prompt validation: %w", err)
	}

	//nolint:gomnd
	cmdRequiredArgsCount := map[string]int{
		"truncate": 0,
		"get":      1,
		"delete":   1,
		"set":      2,
	}

	if len(cmd) > 0 {
		argsCount, ok := cmdRequiredArgsCount[strings.ToLower(cmd[0])]
		if !ok {
			return errors.New("invalid command")
		}
		if len(cmd)-1 != argsCount {
			return errors.New("invalid arguments count")
		}
	}

	return nil
}

// getPromptTemplates returns visualization template for prompt.
func getPromptTemplates() *promptui.PromptTemplates {
	return &promptui.PromptTemplates{
		Prompt:  "{{ . }} ",
		Valid:   "{{ . | green }} ",
		Invalid: "{{ . | red }} ",
		Success: "{{ . | bold }} ",
	}
}

// prepareCommand returns command parts without useless spaces.
func prepareCommand(cmd string) ([]string, error) {
	r := csv.NewReader(strings.NewReader(cmd))
	r.Comma = ' '
	cmdPartsRaw, err := r.Read()
	if err != nil {
		return nil, fmt.Errorf("error while reading prompt command fields: %w", err)
	}

	cmdParts := make([]string, 0, len(cmdPartsRaw))
	for _, part := range cmdPartsRaw {
		if part != "" {
			cmdParts = append(cmdParts, part)
		}
	}

	return cmdParts, nil
}

// handleCommand is used to handle input command.
func handleCommand(client *bongodb.Client, cmd []string) {
	if len(cmd) == 0 {
		return
	}

	switch strings.ToLower(cmd[0]) {
	case "set":
		res := client.Set(&bongodb.Request{
			Key:   cmd[1],
			Value: cmd[2],
		})
		if res.Error != "" {
			fmt.Printf("Error executing set: %s\n", res.Error)
		}
	case "get":
		res := client.Get(&bongodb.Request{
			Key: cmd[1],
		})
		if res.Error != "" {
			fmt.Printf("Error executing get: %s\n", res.Error)
		} else {
			fmt.Println(res.Value)
		}
	case "delete":
		res := client.Delete(&bongodb.Request{
			Key: cmd[1],
		})
		if res.Error != "" {
			fmt.Printf("Error executing delete: %s\n", res.Error)
		} else {
			fmt.Printf("Key '%s' is deleted\n", cmd[1])
		}
	case "truncate":
		res := client.Truncate(&bongodb.Request{})
		if res.Error != "" {
			fmt.Printf("Error executing truncate: %s\n", res.Error)
		} else {
			fmt.Println("Store is cleared")
		}
	default:
		fmt.Println("Unrecognized command")
	}
}
