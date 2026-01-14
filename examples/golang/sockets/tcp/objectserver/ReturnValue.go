package main

type ValueStore struct {
	ReturnValue []int
}

func NewValueStore() *ValueStore {
	vs := ValueStore{}
	return &vs
}
