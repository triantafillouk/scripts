package main

/*
 This is a test reverse proxy, it listens to port 8080 and communicates it back to port 8888.
 It checks the for a specicic string in the response body and if found it sends back a 503 error (service unavailable)

*/


import (
	"flag"
	"io"
	"log"
	// "net"
	"net/http"
	"strings"
	"fmt"
	"net/url"
)

const err_string = "107"

// Hop-by-hop headers. These are removed when sent to the backend.
// http://www.w3.org/Protocols/rfc2616/rfc2616-sec13.html
var hopHeaders = []string{
	"Connection",
	"Keep-Alive",
	"Proxy-Authenticate",
	"Proxy-Authorization",
	"Te", // canonicalized version of "TE"
	"Trailers",
	"Transfer-Encoding",
	"Upgrade",
}

func copyHeader(dst, src http.Header) {
	log.Println("copyHeader ----->");
	for k, vv := range src {
		log.Println(" + ",k,vv)
		for _, v := range vv {
			log.Println(" +> ",k," : ",v)
			
			dst.Add(k, v)
		}
	}
}

func delHopHeaders(header http.Header) {
	for _, h := range hopHeaders {
		header.Del(h)
	}
}

func appendHostToXForwardHeader(header http.Header, host string) {
	// If we aren't the first proxy retain prior
	// X-Forwarded-For information as a comma+space
	// separated list and fold multiple headers into one.
	if prior, ok := header["X-Forwarded-For"]; ok {
		host = strings.Join(prior, ", ") + ", " + host
	}
	header.Set("X-Forwarded-For", host)
}

type proxy struct {
}

func (p *proxy) ServeHTTP(wr http.ResponseWriter, req *http.Request) {
	log.Println("Receive from: ",req.RemoteAddr, ", method: ", req.Method, ",path: ", req.URL)
	log.Println("	< scheme: ",req.URL.Scheme,", host: ",req.Host);

		// http.Error(wr, msg, http.StatusBadRequest)
	log.Println("	< request         URI: ",req.RequestURI)
	log.Println("	< request remote addr: ",req.RemoteAddr) 
	/* Modify request and send it to back server!  */
	var backURI = req.RequestURI
	req.RequestURI = ""
	backPort := 8888

	requestURL := fmt.Sprintf("http://localhost:%d%s", backPort, backURI)

	delHopHeaders(req.Header)
/*
	clientIP, port, err := net.SplitHostPort(req.RemoteAddr); 
	if err == nil {
		appendHostToXForwardHeader(req.Header, clientIP)
		log.Println("- back ip",clientIP)
		log.Println("- port",port)
	}
*/
	// req.URL.Host = "127.0.0.1:8888"
	// req.Host = "127.0.0.1:8888"
	req.Host = requestURL 
	req.URL,_= url.Parse(requestURL)
// req.Proto = "HTTP/1.2"
	log.Println("- host: ",req.Host)
	log.Println("- method: ",req.Method)
	log.Println("- request: ",req);
	log.Println("- header: ",req.Header)
	log.Println("- lenght: ",req.ContentLength)
	log.Println("- values: ",req.Form)
	log.Println("- remote: ",req.RemoteAddr)
	req.ContentLength=0

	client := &http.Client{}
	resp, err := client.Do(req)

//		resp, err := http.Get(requestURL)

	if err != nil {
		http.Error(wr, "Server Error", http.StatusInternalServerError)
		log.Fatal("ServeHTTP:", err)
	}
	log.Println("	> resp status: ",resp.Status,resp.StatusCode)
	// resp.StatusCode=202
	respBody, err := io.ReadAll(resp.Body)


	// res := strings.Contains(string(respBody),"><i4>1007</i4>")
	res := strings.Contains(string(respBody),err_string)
	if(res) {
		resp.StatusCode = 503
	}
	if err != nil {
		http.Error(wr, "Server Error", http.StatusInternalServerError)
		log.Fatal("ServeHTTP:", err)
	}
	defer resp.Body.Close()

	log.Println(req.RemoteAddr, " ", resp.Status)

	delHopHeaders(resp.Header)

	copyHeader(wr.Header(), resp.Header)
	wr.WriteHeader(resp.StatusCode)
// 	io.Copy(wr, respBody)
	fmt.Printf("client: response body: %s\n", respBody)
	wr.Write(respBody)
}

func main() {
	var addr = flag.String("addr", "127.0.0.1:8080", "The addr of the application.")
	flag.Parse()
	
	handler := &proxy{}
	
	log.Println("Starting proxy server on", *addr)
	if err := http.ListenAndServe(*addr, handler); err != nil {
		log.Fatal("ListenAndServe:", err)
	}
}