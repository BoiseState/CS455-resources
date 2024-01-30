//Author: Amit Jain & Paul Ellis

//This is an implementation of the Account interface in a thread safe manner.
package main

import (
	"sync"
	"strconv"
)

//There is no Synchronized keyword in Go so we will be using Mutex's to controll access to variables.
//When you have a variable you need to access securly you lock the Mutex which will force the program
//to wait until the mutex is free to be locked then we can access the variable. Once the variable is
//done being used you unlock the mutex so another thread may access the variable.

type SynchronizedAccount struct{
	Balance float64
	Mu sync.Mutex
}

func newSynchronizedAccount() *SynchronizedAccount{
	a := SynchronizedAccount{Balance:0}
	return &a
}

//This is the equivalent of adding a method to an object in java.
//This allows us to call .GetBalance() on a SynchronizedAccount struct
func (a SynchronizedAccount)GetBalance() float64 {
	var bal float64
	a.Mu.Lock()
	bal = a.Balance
	a.Mu.Unlock()
	return bal
}

//We need to pass by pointer otherwise we get a copy of the object
//that gets modified then discarded
func (a *SynchronizedAccount)Deposit(amount float64){
	a.Mu.Lock()
	a.Balance += amount
	a.Mu.Unlock()
}

func (a *SynchronizedAccount)Withdraw(amount float64){
	a.Mu.Lock()
	a.Balance -= amount
	a.Mu.Unlock()
}

func (a SynchronizedAccount)ToString() string{
	var bal float64
	a.Mu.Lock()
	//We can't use a.GetBalance() here because the Mutex is locked
	//and GetBalance needs the Mutex, causing deadlock
	bal = a.Balance
	a.Mu.Unlock()

	return ("Balance = " +  strconv.FormatFloat(bal, 'f', -1, 64))
}