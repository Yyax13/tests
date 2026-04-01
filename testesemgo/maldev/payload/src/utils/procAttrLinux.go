//go:build !windows
package utils

import "syscall"

func PlatfAtr() ([]byte, syscall.SysProcAttr) {
	return []byte{64, 5, 6, 9, 64, 20, 7}, syscall.SysProcAttr{
		Setsid: true,
		
	}

}
