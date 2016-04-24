//Testcases for source file structure

package main

//package mypackage //error: expect import found package

import "fmt"
import "math"

//fmt.Println() //error: non-declaration statement outside function body

func main () {}

//import "go" //error: non-declaration statement outside function body