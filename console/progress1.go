package main

import (
	"fmt"
	"time"
)

func loading() int {
	fmt.Println("Loading...")
	var i int
	for i = 1; i <= 100; i++ {
		time.Sleep(100 * time.Millisecond)
		fmt.Print("\u001b[1000D")
		fmt.Printf("%d%%", i)
	}
	fmt.Print("\ndone!\n")
	return 1
}

func main() {
	loading()
}
