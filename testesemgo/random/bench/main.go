package main

import (
	"fmt"
	"time"
)

type Microsecond 	int 
type Millisecond 	int 
type Second 		int 
type Minute 		int
var MicrosecondVal 	int
var MillisecondVal	int
var SecondVal		int
var MinuteVal		int

func BusySleepMicrosecond(time Microsecond) {
	for range time {
		var i int
		for i < 28 {
			i++

		}

	}

}

func BusySleepMillisecond(time Millisecond) {
	for range time {
		var i int
		for i < 28425 {
			i++

		}

	}

}

func BusySleepSecond(time Second) {
	for range time {
		var i int
		for i < 28425242 {
			i++

		}

	}

}

func BusySleepMinute(time Minute) {
	for range time {
		var i int
		for i < 28425241615 {
			i++

		}

	}

}

func main() {
	var media []time.Duration = make([]time.Duration, 0)
	for range 125 {
		var i int
		s := time.Now()
		for i < 1 {
			i++

		}

		e := time.Now()
		diff := e.Sub(s)
		media = append(media, diff)

	}

	start := time.Now()
	BusySleepSecond(1)
	end := time.Now()
	duration := end.Sub(start)
	fmt.Println(duration)

}
