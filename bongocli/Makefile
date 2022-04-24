build: .check .build

.build:
	go build -o bin/bongocli main.go

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
