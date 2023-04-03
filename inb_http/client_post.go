package main

/*
	a post request in go
*/

import (
	"flag"
    "encoding/json"
    "fmt"
    "log"
    "net/http"
    "net/url"
	"io"
	"os"
)

func main() {
	serverPort := flag.Int("port",8080,"use server port");
	host := flag.String("host","localhost","hostname");
	flag.Parse()

    data := url.Values{
        "name":       {"John Doe"},
        "occupation": {"gardener"},
    }
	posturl := fmt.Sprintf("http://%s:%d/post", *host, *serverPort)
	fmt.Println("post request to:",posturl,*serverPort)
    resp, err := http.PostForm(posturl, data)

    if err != nil {
        log.Fatal(err)
    }

    var res map[string]interface{}
	fmt.Println(resp.Header)
	fmt.Printf("client, status: %s code: %d\n", resp.Status,resp.StatusCode)

	resBody, err := io.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("client: could not read response body: %s\n", err)
		os.Exit(1)
	}
	fmt.Printf("client: response body: %s\n", resBody)
	if(false){
    json.NewDecoder(resp.Body).Decode(&res)

    fmt.Println(res["form"])
	}
}