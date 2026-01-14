# Go Crash Course

## Variables
In Go there are a few ways to create variables and it can be a bit confusing at first. Here are some pointers on what to do if you get stuck.

> New Variable: `varName := data` 

> Multiple Variables: `var1, var2 := data, data`

This is important because functions can return multiple values such as data and an error. If one of the variables is new use the `:=` operator but if you are reusing all variables just use the `=` operator

> Reassigning variable: `existingVar = new data`

> Ignore one variable of output: `varData, _ := os.Open("/tmp/data")`

This can be usefull if a function returns multiple values but you only care about one. the `_` will let go know that you don't want to save the data from the function call. Here we are ignoring the error value returned from opening a file.

> Declare variable but do not initialize it: `var x int`

This can be useful if you want to declare a value and its type before you know what it will contain. Be careful with this though because it can lead to issues with function calls like Reading from a file.

## Pointers
Yes, go is a language that uses the bane of all programers existences pointers. You don't need to worry about it too much however as go is smart about their use. Most variables will be garbage collected when they are no longer needed so memory management such as in C is unnecessary. However if you want to change a variable within a function like you would in Java you may need to pass in the pointer to a variable using the `&variableName` syntax to pass by reference. In order to dereference sometimes it may be necessary to use the `*variableName` syntax as well. You can also tell variables that they will be a pointer type by declaring it as `var pointerName *variableType`

## Loops
In Go you will find that you can only use for loops. No while loops or do while loops exist. With for loops however you can achieve the same effects and much more with a bit of creative thinking. As an example rather than doing a `while(true){}` loop, in go just use `for {}` to create an infinite loop.


## Final Remarks
This is just a surface scratch of information on Go. The topics here are some that I thought might be usefull to explain in a different manner than the official documentation. If there are other concepts that you would like to learn (or you just don't understand what I am getting at) please refer to the Go documentation, there are examples and much more in depth tutorials that you can visit there.

* [Go Homepage](https://go.dev)
* [Go Examples](https://gobyexample.com/)
* [Go Playground for testing small programs](https://go.dev/play/)
* [Go Tour](https://go.dev/tour/list)
* [Go Textbook Recomended](https://www.amazon.com/Programming-Language-Addison-Wesley-Professional-Computing/dp/0134190440)
