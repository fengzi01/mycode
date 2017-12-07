package main

import (
	"fmt"
	"io"
	"net"
)

type Proxy struct {
}

// 创建proxy
func NewProxy() *Proxy {
	return &Proxy{}
}

func (p *Proxy) ProxyConn(conn net.Conn) {
	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil && err != io.EOF {
		panic(err)
	}
	fmt.Println("received:", buf[:n])

	buf[0] = 5
	buf[1] = 0
	conn.Write(buf[:2])
	fmt.Println("send:", buf[:2])
}

func handleConnection(conn net.Conn) {
	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil && err != io.EOF {
		panic(err)
	}
	fmt.Println("received:", buf[:n])

	buf[0] = 5
	buf[1] = 0
	conn.Write(buf[:2])
	fmt.Println("send:", buf[:2])

	n, err = conn.Read(buf)
	if err != nil && err != io.EOF {
		panic(err)
	}
	fmt.Println(buf[:n])

	conn.Close()
}

func main() {
	ln, err := net.Listen("tcp", ":1080")
	if err != nil {
		fmt.Println("bind error!", err)
		return
	}

	fmt.Println("listen on: 1080")

	for {
		conn, err := ln.Accept()
		if nil != err {
			fmt.Println("accept error!", err)
			continue
		}

		go handleConnection(conn)
	}
}
