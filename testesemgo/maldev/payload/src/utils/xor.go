package utils

func Xor(p, kk []byte) []byte {
	k := kk
	xs := make([]byte, len(p))
	for i := range p {
		xs[i] = p[i] ^ k[(i+1)%len(k)]
		k[(i+2)%len(k)] = k[i%len(k)]

	}

	return xs

}

var XorKey []byte = []byte{103, 111, 103, 111, 103, 111, 103, 111, 103, 111}