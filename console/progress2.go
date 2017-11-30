package main

import (
	"fmt"
	"strings"
	"time"
)

func loading() int {
	fmt.Println("Loading...")
	var i, width int
	var left, right string
	for i = 1; i <= 100; i++ {
		width = i / 2
		left = "[" + strings.Repeat("#", width)
		right = strings.Repeat(" ", 50-width) + "]"
		fmt.Printf("\u001b[1000D%s%s", left, right)
		time.Sleep(100 * time.Millisecond)
	}
	fmt.Print("\nok!\n")
	return 1
}

func main() {
	loading()
}
