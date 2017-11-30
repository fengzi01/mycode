package main

import (
	"fmt"
	_ "github.com/lunux2008/xulu"
	"math/rand"
	"strconv"
	"strings"
	"time"
)

func loading(count int) int {
	fmt.Print(strings.Repeat("\n", count))     // 初始化进度条所占的空间
	var allProgress []int = make([]int, count) // [0,0,0,0]
	for {
		time.Sleep(10 * time.Millisecond)
		// 随机选择一个进度条，增加进度
		var unfinished []int = make([]int, 0)
		for i, progress := range allProgress {
			if progress < 100 {
				unfinished = append(unfinished, i)
			}
		}
		if len(unfinished) <= 0 {
			break
		}

		// 在未完成的progress中随机选取一个 + 1
		var index int = unfinished[rand.Intn(len(unfinished))]
		allProgress[index] += 1 // 进度+1

		// 绘制进度条
		fmt.Print("\u001b[1000D")                        // 移动到最左边
		fmt.Print("\u001b[" + strconv.Itoa(count) + "A") // 往上移动
		for _, progress := range allProgress {
			width := progress / 2
			left := "[" + strings.Repeat("#", width)
			right := strings.Repeat(" ", 50-width) + "]"
			fmt.Println(left + right)
		}
	}
	return 1
}

func main() {
	loading(10)
}
