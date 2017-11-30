package main

import (
	"fmt"
)

func main() {
	fmt.Println("\u001b[30m A \u001b[31m B \u001b[32m C \u001b[33m D \u001b[0m")
	fmt.Println("\u001b[34m E \u001b[35m F \u001b[36m G \u001b[37m H \u001b[0m")
	fmt.Println("\u001b[30;1m A \u001b[31;1m B \u001b[32;1m C \u001b[33;1m D \u001b[0m")
	fmt.Println("\u001b[34;1m E \u001b[35;1m F \u001b[36;1m G \u001b[37;1m H \u001b[0m")
}
