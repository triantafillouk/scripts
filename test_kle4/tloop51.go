
package main

import "fmt"
import "os"

func main() {
    fmt.Println("Start fori test loop")

	var(
	p float64 =0.0
	l float64 =0.0
	k3 float64 =32.1
	)

	for i := 0; i < 101; i++ {
		for j :=1; j< 800001 ; j++ {
			l = (float64)(i*j)+k3
			p = p + l
		}
		fmt.Printf("loop:%v = %v\n",i,p)
	}
	fmt.Fprintf(os.Stderr,"End test: p=%v\n",p)
	
}