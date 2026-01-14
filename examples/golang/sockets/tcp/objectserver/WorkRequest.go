package main

type WorkRequest interface {
	Execute() any
}
