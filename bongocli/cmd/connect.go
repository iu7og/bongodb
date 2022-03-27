// Package cmd represents all CLI commands.
package cmd

import (
	"context"
	"encoding/csv"
	"errors"
	"fmt"
	"io"
	"strings"

	"github.com/iu7og/bongodb/bongocli/internal/connect"
	"github.com/iu7og/bongodb/bongocli/internal/pb/bongodb"
	"github.com/manifoldco/promptui"
	"github.com/spf13/cobra"
	"google.golang.org/grpc/status"
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
		return fmt.Errorf("error while connecting to bongodb instance in connect command, error is: %v", err)
	}

	prompt := promptui.Prompt{
		Label:     ">",
		Validate:  validateCommand,
		Templates: getPromptTemplates(),
	}

	fmt.Println("Use Ctrl+C to exit")

	ctx := context.Background()
	for {
		rawCmd, err = prompt.Run()
		if err != nil {
			if err == promptui.ErrInterrupt || err == promptui.ErrEOF {
				fmt.Println("Exiting")

				return nil
			}

			return fmt.Errorf("error while running prompt command in connect command, error is: %v", err)
		}

		cmd, err = prepareCommand(rawCmd)
		if err != nil {
			return fmt.Errorf("error while preparing prompt command in connect command, error is: %v", err)
		}

		handleCommand(ctx, client, cmd)
	}
}

// validateCommand is used for on-fly validation of input command.
func validateCommand(rawCmd string) error {
	cmd, err := prepareCommand(rawCmd)
	if err != nil {
		if strings.HasSuffix(err.Error(), io.EOF.Error()) {
			return errors.New("command can't be empty")
		}

		return fmt.Errorf("error while preparing prompt command in prompt validation, error is: %v", err)
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
		return nil, fmt.Errorf("error while reading prompt command fields, error is: %v", err)
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

	cmd[0] = strings.ToLower(cmd[0])

	switch cmd[0] {
	case "set":
		_, err := client.Set(ctx, &bongodb.KeyValueRequest{
			Key:   &wrapperspb.StringValue{Value: cmd[1]},
			Value: &wrapperspb.StringValue{Value: cmd[2]},
		})
		if err != nil {
			fmt.Printf("Error while executing set, error is: %s\n", status.Convert(err).Message())
		}
	case "get":
		res, err := client.Get(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: cmd[1]}})
		if err != nil {
			fmt.Printf("Error while executing get, error is: %s\n", status.Convert(err).Message())
		} else {
			fmt.Println(res.GetValue().GetValue())
		}
	case "delete":
		_, err := client.Delete(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: cmd[1]}})
		if err != nil {
			fmt.Printf("Error while executing delete, error is: %s\n", status.Convert(err).Message())
		} else {
			fmt.Printf("Key '%s' is deleted\n", cmd[1])
		}
	case "truncate":
		_, err := client.Truncate(ctx, &emptypb.Empty{})
		if err != nil {
			fmt.Printf("Error while executing truncate, error is: %s\n", status.Convert(err).Message())
		} else {
			fmt.Println("Store is cleared")
		}
	}
}
