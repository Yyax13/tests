package main

import (
	"crypto/rand"
	"fmt"
	mrand "math/rand/v2"
)

type encoderType struct{}
var encoder encoderType

func randomByteSliceRange(n int, min, max byte) []byte {
	buf := make([]byte, n)
	for i := 0; i < n; i++ {
		var b [1]byte
		_, err := rand.Read(b[:])
		if err != nil {
			i--
			continue

		}

		buf[i] = min + (b[0] % (max - min + 1))

	}

	return buf

}

func (e encoderType) encode(data []byte) []byte {
	result := make([]byte, 0)
	for _, b := range data {
		block := make([]byte, 0)

		block = append(block, 1) // Start of block 

			block = append(block, 7) // Start of i-subBlock

				n := mrand.IntN(2) + 1
				iBlock := randomByteSliceRange(n, 48, 90)
				block = append(block, iBlock...)

			block = append(block, 5) // End of i-subBlock

			block = append(block, 3) // Start of dataBlock

				size := (len(iBlock) + int(b))
				dataBlock := randomByteSliceRange(size, 91, 122)
				block = append(block, dataBlock...)
			
			block = append(block, 6) // End of dataBlock

		block = append(block, 4) // End of block
		result = append(result, block...)

	}

	return result

}

func (e encoderType) decode(data []byte) []byte {
	result := make([]byte, 0)
	for i := 0; i < len(data); {
		i++ // Skip start of block ( 1 )
		i++ // Skip start of i-subBlock ( 7 )

		iBlockLen := 0
		for data[i] != 5 {
			iBlockLen++
			i++

		}

		i++ // Skip end of i-subBlock
		i++ // Skip start of dataBlock

		dataBlockLen := 0
		for data[i] != 6 {
			dataBlockLen++
			i++

		}

		i++ // Skip end of dataBlock
		i++ // Skip end of block

		b := (dataBlockLen - iBlockLen)
		result = append(result, byte(b))
		
	}

	return result

}

func main() {
	testString := "a"
	fmt.Printf("Original value: %s (bytes: <% 02x>)\nOriginal len: %d\n\n", testString, []byte(testString), len(testString))

	encoded := encoder.encode([]byte(testString))
	fmt.Printf("Encoded value: <% 02x...>\nEncoded len: %d\n\n", encoded[:7], len(encoded))

	decoded := encoder.decode([]byte(encoded))
	fmt.Printf("Decoded value: %s (bytes: <% 02x>)\nDecoded len: %d\n\n", string(decoded), decoded, len(decoded))

}