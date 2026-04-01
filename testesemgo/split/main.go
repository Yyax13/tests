package main

import (
	"fmt"
	"os"
)

func sprintBytes(data []byte) string {
	r := ""
	ii := 0
	for i, b := range data {
		if i == 0 {
			r += fmt.Sprintf("{\n    0x%02X", b)
			continue

		}

		if ii == 10 {
			ii = 0
			r += fmt.Sprintf(",\n    0x%02X", b)
			continue

		} 

		r += fmt.Sprintf(", 0x%02X", b)
		ii++

	}

	r += ",\n}\n"

	return r

}

func split(data []byte, chunkSize int) [][]byte {
	result := make([][]byte, 0)
	
	count := int(len(data) / chunkSize) + 1
	var _a int
	var _oa int
	for i := range count {	
		_t_cond := (i + 1) * chunkSize < len(data) - _a
		_t_resultIndBound := map[bool]int{true: (i + 1) * chunkSize, false: len(data) - _oa}[_t_cond]
		_t_result := data[_oa:_t_resultIndBound]
		
		result = append(result, _t_result)
		fmt.Printf("var p%d []byte = []byte%s\n", i + 1, sprintBytes(_t_result))
	
		_oa = _a
		_a = _a + _t_resultIndBound
			
	}

	return result

}

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Usage: \n   %s <file_absolute_path>\n\n", os.Args[0])
		return

	}

	myBuf, err := os.ReadFile(os.Args[1])
	if err != nil {
		fmt.Printf("Failed to read: %s", err.Error())
		return

	}

	fmt.Print("package p\n\n")
	_ = split(myBuf, 40000)

}
