package main

import (
	"fmt"
	"os"
	"strings"
	"time"
)

func xor(plain, key []byte) (xor []byte) {
	xorStr := make([]byte, len(plain))
	time.Sleep(25 * time.Millisecond)
	for i := 0; i < len(plain); i++ {
		xorStr[i] = plain[i] ^ key[(i+1)%len(key)]
		key[(i+2)%len(key)] = key[i%len(key)]

	}

	return xorStr

}

func genFlag(key, validPass []byte) string {
	wna := xor(key, validPass)
	time.Sleep(400 * time.Millisecond)

	swna3 := string(wna)
	var nwna3n12 []string
	for _, v := range swna3 {
		nwna3n12 = append(nwna3n12, string([]rune{v - 18}))

	}

	return fmt.Sprintf("%sbsw%s%s%s", string(key), strings.Join(nwna3n12, ""), "h", string(wna))

}

func validate() {
	s := time.Now()
	genFlag([]byte("KWubqouwidbWD@#ncai#&!@*n31231"), []byte("howoisntlammer"))
	e := time.Since(s)
	if e > time.Millisecond*430 {
		fmt.Print("Hello Debugger!")
		os.Exit(1)

	}

}

func fmtPrintln(pass string) (bool, []byte) {
	validate()
	var passProcess1stStep []string
	for _, v := range pass {
		passProcess1stStep = append(passProcess1stStep, string([]rune{v + 3}))

	}

	newPass := fmt.Sprintf("new:%s", strings.Join(passProcess1stStep, ""))
	key := []byte(pass)
	plain := []byte(newPass)
	xorString := xor(plain, key)

	return xorString[len(plain)%len(key)] == 18+key[int(plain[len(plain)-1])%len(key)], xorString

}

func fmtSprintf(x []byte) []byte {
	f := []byte{72, 101, 108, 108, 111, 32, 109, 111, 109, 109, 121, 44, 32, 73, 32, 99, 97, 110, 32, 100, 111, 32, 114, 101, 118, 101, 114, 115, 101, 32, 101, 110, 103, 105, 110, 101, 101, 114, 105, 110, 103, 32, 111, 110, 32, 97, 110, 121, 116, 104, 105, 110, 103, 32, 110, 111, 119, 33}
	return xor(f, x)

}

func main() {
	validate()
	if len(os.Args) < 2 {
		fmt.Println("Hello World!")
		return

	}

	validate()
	b, bb := fmtPrintln(os.Args[1])
	validate()
	if b {
		validate()
		bbb := fmtSprintf(bb)
		validate()
		fmt.Printf("Hello hoWo, here is your flag:\n	%v", bbb)
		return

	} else {
		fmt.Println("Hello World!")
		return

	}

}
