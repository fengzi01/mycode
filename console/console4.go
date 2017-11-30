package main

import (
	"fmt"
)

func main() {
	fmt.Println("\u001b[44m A \u001b[45m B \u001b[46m C \u001b[47m D \u001b[0m")
	fmt.Println("\u001b[40m A \u001b[41m B \u001b[42m C \u001b[43m D \u001b[0m")
	fmt.Println("\u001b[40;1m A \u001b[41;1m B \u001b[42;1m C \u001b[43;1m D \u001b[0m")
	fmt.Println("\u001b[44;1m A \u001b[45;1m B \u001b[46;1m C \u001b[47;1m D \u001b[0m")

}
