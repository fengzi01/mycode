package main

import (
	"fmt"
)

func main() {
	var i, j int
	for i = 0; i < 16; i++ {
		for j = 0; j < 16; j++ {
			code := i*16 + j
			fmt.Printf("\u001b[38;5;%dm%-4d", code, code)
		}
		fmt.Println("\u001b[0m")
	}

}
