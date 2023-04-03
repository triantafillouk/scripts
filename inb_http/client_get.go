package main

/*
	A get request in go
*/

import (
	"flag"
	"fmt"
	// "io"
	"log"
	// "net"
	"net/http"
	// "strings"
	// "errors"
	// "time"
	// "strconv"
	"io"
	"os"
)


func main() {
	serverPort := flag.Int("port",8080,"use server port");
	host := flag.String("host","localhost","hostname");
	flag.Parse()

	requestURL := fmt.Sprintf("http://%s:%d/test", *host, *serverPort)
	log.Println("request is ",requestURL)
	res, err := http.Get(requestURL)
	if err != nil {
		fmt.Printf("error making http request: %s\n", err)
		os.Exit(1)
	}

	fmt.Printf("client: got response!\n")
	fmt.Printf("client, status: %s code: %d\n", res.Status,res.StatusCode)

	resBody, err := io.ReadAll(res.Body)
	if err != nil {
		fmt.Printf("client: could not read response body: %s\n", err)
		os.Exit(1)
	}
	fmt.Printf("client: response body: %s\n", resBody)

}