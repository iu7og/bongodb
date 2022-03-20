package main

import (
	"context"
	"flag"
	"log"
	"time"

	"github.com/iu7og/bongodb/bongocli/internal/pb/bongodb"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/protobuf/types/known/wrapperspb"
)

const timeoutValue = 15

var (
	addr string
)

func init() {
	flag.StringVar(&addr, "addr", "localhost:5000", "server address in the format of host:port")
}

func main() {
	flag.Parse()

	conn, err := grpc.Dial(addr, grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	defer func(conn *grpc.ClientConn) {
		errr := conn.Close()
		if errr != nil {
			log.Fatalf("failed to close: %v", errr)
		}
	}(conn)
	c := bongodb.NewBongoDBClient(conn)

	ctx, cancel := context.WithTimeout(context.Background(), timeoutValue*time.Second)
	defer cancel()

	r, err := c.Get(ctx, &bongodb.KeyRequest{Key: &wrapperspb.StringValue{Value: "someKey"}})
	if err != nil {
		log.Fatalf("cant get key: %v", err)
	}
	log.Printf("Got value: %s", r.GetValue())
}
