package main

import (
	//	"file"
	"fmt"
)

// defer 表示在函数退出时调用
// 只能调用一行？
//func ReadFile(filePath string) {
//    file.Open(filePath)
//    defer file.Close()

//    if true {
//        file.Read()
//    } else {
//        return false
//    }
//}

func main() {

	f()
	fmt.Println("Returned normally from f.")
}

func f() {
	defer func() {
		// recover相当于接受到异常
		if r := recover(); r != nil {
			fmt.Println("Recovered in f", r)
		}
	}()

	fmt.Println("Calling g.")
	g(0)
	// 正常情况返回
	fmt.Println("Returned normally from g.")
}

func g(i int) {
	if i > 3 {
		fmt.Println("Panicking!")
		// panic 相当于throw 一个异常
		panic(fmt.Sprintf("%v", i))
	}

	defer fmt.Println("Defer in g", i)

	fmt.Println("Printing in g", i)
	g(i + 1)

	fmt.Println("Returned normally in g.")
}
