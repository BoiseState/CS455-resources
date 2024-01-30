//Author: Amit Jain & Paul Ellis

package main

import (
	"strconv"
)

//Interface for an Account, this allows us to create basic accounts
//and synchronized accounts
//Below the interface is an example of an implementation of the interface
type Account interface{
	GetBalance() float64
	Deposit(amount float64)
	Withdraw(amount float64)
	ToString() string
}

type BasicAccount struct{
	Balance float64
}

func newBasicAccount() *BasicAccount{
	a := BasicAccount{Balance:0}
	return &a
}

func (a BasicAccount)GetBalance() float64 {
	return a.Balance
}

func (a *BasicAccount)Deposit(amount float64){
	a.Balance = a.Balance + amount
}

func (a *BasicAccount)Withdraw(amount float64){
	a.Balance = a.Balance - amount
}

func (a BasicAccount)ToString() string{
	return ("balance = " + strconv.FormatFloat(a.GetBalance(), 'f', -1, 64))
}