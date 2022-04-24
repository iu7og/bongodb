// Package connect is used to handle gRPC connection related work.
package connect

import (
	"fmt"

	"github.com/iu7og/bongodb/bongocli/internal/pb/bongodb"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

// Connect returns instance of BongoDB client.
func Connect(addr string) (bongodb.BongoDBClient, error) {
	conn, err := grpc.Dial(addr, grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		return nil, fmt.Errorf("failed to connect to bongodb server: %w", err)
	}

	return bongodb.NewBongoDBClient(conn), nil
}
