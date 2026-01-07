package main

import (
	"fmt"

	"rsc.io/quote"
)

func hello() string {
	return quote.Hello()
}

func main() {
	fmt.Println(hello())
}
