package main

import (
	"fmt"
)

const (
	_        = iota
	KB int64 = 1 << (iota * 10)
	MB
	GB
	TB
	PB
	EB
)

func main() {
	fmt.Println(KB, MB, GB, TB, PB, EB)
}
