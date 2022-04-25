package bongodb

// OperationType represents type of executed operation.
type OperationType uint8

//nolint:revive
const (
	Set OperationType = iota
	Get
	Delete
	Truncate
)

// Request represents request object to BongoDB.
type Request struct {
	OperationType OperationType `json:"operation_type"`
	Key           string        `json:"key"`
	Value         string        `json:"value"`
}

// Response represents response object from BongoDB.
type Response struct {
	Error string `json:"error"`
	Value string `json:"value"`
}
