package main

import (
	"fmt"
)

func main() {
	var m int = 10

	_ = m

	// 多个变量声明并初始化
	var i, j, k = 1, 2, 3

	_, _, _ = i, j, k

	// 声明多个变量
	var (
		no   int
		name string
	)

	_ = no
	_ = name

	// := 隐含声明变量并赋值
	str := "www.baidu.com"

	_ = str

	// Go中有个特殊的变量 _
	_, ret := 2, 3

	_ = ret

	// 定义常量
	const const1 int = 4
	const const2 bool = true
	const (
		const3 int  = 10
		const4 bool = true
	)

	_ = GetMsg(const3)
}

// 函数
func GetMsg(i int) (str string) {
	fmt.Println("i:", i)
	str = "www.baidu.com"
	return str
}

// 无法重新定义
func GetMsg1(i int) (str string, err string) {
	fmt.Println("i:", i)
	str = "www.baidu.com"
	return str, str
}
