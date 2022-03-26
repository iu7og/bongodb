// Package cmd represents all CLI commands.
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
		client bongodb.BongoDBClient
		rawCmd string
		cmd    []string
	)

	client, err = connect.Connect(args[0])
	if err != nil {
		return err
	}

	prompt := promptui.Prompt{
		Label:     ">",
		Validate:  validateCommand,
		Templates: getPromptTemplates(),
	}

	ctx := context.Background()
	for {
		rawCmd, err = prompt.Run()
		if err != nil {
			return err
		}

		cmd, err = prepareCommand(rawCmd)
		if err != nil {
			return err
		}

		handleCommand(ctx, client, cmd)
	}
}

// validateCommand is used for on-fly validation of input command.
func validateCommand(rawCmd string) error {
	cmd, err := prepareCommand(rawCmd)
	if err != nil {
		return err
	}

	//nolint:gomnd
	cmdRequiredArgsCount := map[string]int{
		"truncate": 0,
		"exit":     0,
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
		return nil, err
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
func handleCommand(ctx context.Context, client bongodb.BongoDBClient, cmd []string) {
	if len(cmd) == 0 {
		return
	}

	switch cmd[0] {
	case "exit":
		return
	case "set":
		_, err := client.Set(ctx, &bongodb.KeyValueRequest{
			Key:   &wrapperspb.StringValue{Value: cmd[1]},
			Value: &wrapperspb.StringValue{Value: cmd[2]},
		})
		if err != nil {
			fmt.Printf("Error while executing set, error is: %s\n", err)
		}
	case "get":
		res, err := client.Get(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: cmd[1]}})
		if err != nil {
			fmt.Printf("Error while executing get, error is: %s\n", err)
		} else {
			fmt.Println(res.GetValue().GetValue())
		}
	case "delete":
		_, err := client.Delete(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: cmd[1]}})
		if err != nil {
			fmt.Printf("Error while executing delete, error is: %s\n", err)
		} else {
			fmt.Printf("Key '%s' is deleted\n", cmd[1])
		}
	case "truncate":
		_, err := client.Truncate(ctx, &emptypb.Empty{})
		if err != nil {
			fmt.Printf("Error while executing truncate, error is: %s\n", err)
		} else {
			fmt.Println("Store is cleared")
		}
	}
}
