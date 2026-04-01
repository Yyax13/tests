package main

import (
	"fmt"
	"net"
)

func getPass() string {
	var p1 string = "!"
	var p2 string = "1N9F1N"
	var p3 string = "FPNI3GSA19FNQWK021"
	var p4 string = "c"
	var p5 []byte = []byte("5")
	return p1 + p2 + p3 + p4 + string(p5)

}

func getFlag() string {
	flagPattern := "HOWO{%s}"
	flagContent_1 := "r3v3rs3"
	flagContent_2 := "_eng"
	return fmt.Sprintf(flagPattern, flagContent_1 + flagContent_2)
	
}

func main() {
	fmt.Println("Bem vindo ao CTF: senha-e-flag-2")
	fmt.Print("Insira a senha e obtenha a flag: ")
	
	var inputPass string
	fmt.Scanln(&inputPass)
	if inputPass == getPass() {
		fmt.Printf("Parabéns! Aqui está sua flag:\n%s\n", getFlag())
		
	} else {
		fmt.Println("Que pena :(\nVocê errou, vou mandar isso para o chefe >:(")
		conn, err := net.Dial("tcp", "127.0.0.1:19876")
		if err != nil {
			fmt.Println("Oh, eu não consigo acessar meu chefe também :(")
			return
			
		}
		
		defer conn.Close()
		conn.Write([]byte("Esse cara que você testou é muito lammer >:(\nAche alguém melhor!"))
		return
		
	}
	
}