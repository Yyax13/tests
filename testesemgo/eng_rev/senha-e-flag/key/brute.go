package main

import (
    "fmt"
)

func xor(plain, key []byte) []byte {
    k := make([]byte, len(key))
    copy(k, key)
    out := make([]byte, len(plain))
    for i := 0; i < len(plain); i++ {
        out[i] = plain[i] ^ k[(i+1)%len(k)]
        k[(i+2)%len(k)] = k[i%len(k)]
    }
    return out
}

// replica exatamente a validação que você indicou:
// return xorString[len(plain)%len(key)] == 18 + key[plain[len(plain)-1]], xorString
func validate(pass []byte) bool {
    // monta plain = "new:" + every char de pass +3
    step := make([]byte, 0, len(pass))
    for _, v := range pass {
        step = append(step, v+3)
    }
    plain := append([]byte("new:"), step...)
    xorString := xor(plain, pass)

    idx := len(plain) % len(pass)         // índice do xorString
    rhsIndex := int(plain[len(plain)-1]) // índice usado para acessar key
    if rhsIndex < 0 || rhsIndex >= len(pass) {
        return false
    }
    rhs := byte(18) + pass[rhsIndex]
    return xorString[idx] == rhs
}

func isPrintableNoSpace(b byte) bool {
    return b >= 33 && b <= 126
}

func main() {
    // ajuste esse L se quiser (maior L = mais chances)
    L := 120

    // inicializa pass com 'A'
    pass := make([]byte, L)
    for i := range pass {
        pass[i] = 'A'
    }

    // iteramos sobre valores imprimíveis para o último byte (last)
    for last := byte(33); last <= 126; last++ {
        rhsIndex := int(last) + 3
        // precisa caber dentro do pass e não pode sobrescrever pass[0] ou pass[1]
        if rhsIndex < 2 || rhsIndex >= L {
            continue
        }

        pass[L-1] = last // marca o último byte que define rhsIndex

        // varre p0 e p1 imprimíveis
        for p0 := byte(33); p0 <= 126; p0++ {
            pass[0] = p0
            for p1 := byte(33); p1 <= 126; p1++ {
                pass[1] = p1

                // LHS = (p0+3) ^ p1
                lhs := int((p0 + 3) ^ p1)
                // queremos rhs = 18 + pass[rhsIndex], então:
                needed := lhs - 18
                if needed < 33 || needed > 126 {
                    continue
                }

                // não podemos sobrescrever p0/p1, e garantimos imprimível
                pass[rhsIndex] = byte(needed)

                // valida imprimiridade do vetor inteiro
                ok := true
                for _, b := range pass {
                    if !isPrintableNoSpace(b) {
                        ok = false
                        break
                    }
                }
                if !ok {
                    continue
                }

                // testa a validação real
                if validate(pass) {
                    fmt.Println("[+] Encontrou senha válida (ASCII imprimível, sem espaços):")
                    fmt.Printf("    '%s'\n", string(pass))
                    fmt.Printf("    hex: ")
                    for _, b := range pass {
                        fmt.Printf("%02x", b)
                    }
                    fmt.Println()
                    return
                }
            }
        }
    }

    fmt.Println("[-] Não encontrei nenhuma senha com os parâmetros atuais. Aumenta L ou relaxa o charset.")
}
