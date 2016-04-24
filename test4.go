//Testcases for variable declaration, definition and usage

package main

import "fmt"
import "math"

var a;
var b int;
var c int = 5;

// var a float64 = 1.0; //error: redefine

func main () {
// var d int = (2+3)*(4*(3+4)); //error if one parenthesis is deleted
b = a+5;
// d = a+5; // error: undefinied var
}
