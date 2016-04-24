//Testcases for function declaration and usage

package main

import "fmt"
import "math"

func concat(a string, b string) {
}

//func concat() { } //error: redeclare func

func main () {

var a string;
var b string;

concat(a,b);

//concat(); //error: missing arguments

//concat2(); //error: undefined func

fmt.Println();

//myClass.func(); //error: undefined package
}

