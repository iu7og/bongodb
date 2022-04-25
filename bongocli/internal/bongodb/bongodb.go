// Package bongodb contains BongoDB communication logic.
package bongodb

import (
	"github.com/go-resty/resty/v2"
)

// Client represents client object with HTTP client and connection address.
type Client struct {
	client  *resty.Client
	address string
}

// New creates new instance of client with given address.
func New(address string) *Client {
	return &Client{
		client:  resty.New(),
		address: address,
	}
}

// Set performs Set BongoDB operation.
func (c *Client) Set(req *Request) *Response {
	res := &Response{}

	req.OperationType = Set
	_, err := c.client.R().
		SetBody(req).
		SetResult(res).
		Put(c.address)
	if err != nil {
		res.Error = err.Error()
	}

	return res
}

// Get performs Get BongoDB operation.
func (c *Client) Get(req *Request) *Response {
	res := &Response{}

	req.OperationType = Get
	_, err := c.client.R().
		SetBody(req).
		SetResult(res).
		Post(c.address)
	if err != nil {
		res.Error = err.Error()
	}

	return res
}

// Delete performs Delete BongoDB operation.
func (c *Client) Delete(req *Request) *Response {
	res := &Response{}

	req.OperationType = Delete
	_, err := c.client.R().
		SetBody(req).
		SetResult(res).
		Post(c.address)
	if err != nil {
		res.Error = err.Error()
	}

	return res
}

// Truncate performs Truncate BongoDB operation.
func (c *Client) Truncate(req *Request) *Response {
	res := &Response{}

	req.OperationType = Truncate
	_, err := c.client.R().
		SetBody(req).
		SetResult(res).
		Post(c.address)
	if err != nil {
		res.Error = err.Error()
	}

	return res
}
