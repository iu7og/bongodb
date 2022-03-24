build: .check .build

.build:
	go build -o bin/bongocli main.go

generate: .generate

.generate:
	mkdir -p internal/pb/bongodb
	protoc --go_out=internal/pb/bongodb --go_opt=paths=source_relative \
		--go-grpc_out=internal/pb/bongodb --go-grpc_opt=paths=source_relative \
        -I ../bongodb bongodb.proto

check: .check

.check: .imports .lint .test

.test:
	go test ./...

.lint:
	golangci-lint run --config=../.github/linters/.golangci.yml ./...

.imports:
	find . -type f -name "*.go" -exec gsed -i '/import (/,/)/{ /^[ \t]*$$/d}' {} +
	$(GOPATH)/bin/goimports -w ./

clean:
	rm -f bin/*.exe
