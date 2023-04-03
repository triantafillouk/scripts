package main

import (
	// "errors"
	"fmt"
	"strconv"
	"io"
	"net/http"
	"log"
	// "os"
)

var show bool = true
var i int = 0

func getRoot(w http.ResponseWriter, r *http.Request) {
		
		msg := "request host: "+r.Host+" "
		msg += "	remote: "+r.RemoteAddr+" "
		msg += "	["+r.RequestURI+"]\n"
		fmt.Printf("%s\n",msg)
		log.Println(r.Header)
/*
	if(show) {
		ind_s := strconv.Itoa(i)
		msg := "request host: "+r.Host+" "+ ind_s +" "
		msg += "	remote: "+r.RemoteAddr+" "
		msg += "	["+r.RequestURI+"]\n"
		i += 1
		fmt.Printf(msg)
		if((i % 10)==9) {
			io.WriteString(w, "Error 107 from my website "+ind_s+ "!\n")
		} else {
			io.WriteString(w, "This is my website "+ind_s+ "!\n")
		};
		show = false
	} else {
		show=true
	}
*/
		ind_s := strconv.Itoa(i)
		msg = "serving host: "+r.Host+" "+ ind_s +" "
		msg += "	remote: "+r.RemoteAddr+" "
		msg += "	["+r.RequestURI+"]\n"
		i += 1
		fmt.Printf(msg)
		if((i % 10)==9) {
			io.WriteString(w, "Error 107 from my website "+ind_s+ "!\n")
		} else {
			io.WriteString(w, "This is my website "+ind_s+ "!\n")
		};
}
func getFavicon(w http.ResponseWriter, r *http.Request) {
	fmt.Printf("got /favicon request\n")
	io.WriteString(w, "Hello, HTTP!\n")
}

func main() {
	http.HandleFunc("/", getRoot)
	http.HandleFunc("/favicon.ico", getFavicon)

	err := http.ListenAndServe(":8888", nil)
	log.Println(err)
}