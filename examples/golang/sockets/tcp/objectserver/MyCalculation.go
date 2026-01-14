package main
import "fmt"
type MyCalculation struct {
	Size int
	Values []int
}

func NewMyCalculation(n int) *MyCalculation {
	mc := MyCalculation{Size: n}
	return &mc
}

func (m MyCalculation) Execute() any {
	m.Values = make([]int, m.Size)
	fmt.Println("MyCalculation: ", m.Size)
	for i, _ := range m.Values {
		m.Values[i] = i * i
	}
	return m.Values
}
