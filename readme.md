## What?

_Orava_ is a very small interpreted language that by accident resembles [Forth](https://nl.wikipedia.org/wiki/Forth*%28programmeertaal%29) (I did not know about Forth at the time I made this). It is small enough to fit an Arduino without sacrificing the readability of the code.

## Why?
* because I needed this for a project where I did not have physical access to a microcontroller but could reach it via Bluetooth.
* Because I could.
* Because I wanted to run simple script on an Arduino without a compile, upload cycle.

## How?
(under construction)
Orava is a stack based languages. Commands can place items on the stack and/or take items from the stack for there arguments.

For example:
```
		| Here the stack is empty				|	[]
1		| Places '1' on the stack				|	[1]
3		| Places '3' on the stack				|	[1, 3]
4		| Places '4' on the stack				|	[1, 3, 4]
+   	| Consumes 2 items and adds them		|	[1, 7]
		| and places the answer on them			|
		| on the stack.							|
-   	| Consumes 2 items and subtracts them	|	[1, 7]
		| and places the answer on them			|	[-6]
		| on the stack.							|
print	| Consumes 1 item, and prints it. (-6)	|	[]
```

### Branching
You can branch your code using `then`, `else` and `end`. If the value on the stack is not zero `then` evaluates the code right after
it until it reaches `else`. If the value on the stack is `zero` it evaluates the code from `else` till `end`.

Example:
```
1 1 + 2 equal? then they-are-equal print else they-are-unequal print end
```
This function adds 1 to 1, and checks if the answer is equal to 2, if so
it prints `they-are-equal`, if not (but why :) it prints `they-are-unequal`.

### Iteration
You can put lists with `[` and `]` and iterate over there contents with `iter` and `endit`.


Example:
```
[ 1 2 3 ] iter 1 + print endit
```
Will print:
```
2
3
4
```
TODO: more documentation about lists ...

### Functions
You can put often used sequences of commands in functions using

`: [name of command] [form ...]`.

Example:
```
: add-1-and-print 1 + print
[ 1 2 3 ] iter add-1-and-print endit
```
Will do the same as the previous example.

### Stack manipulation
The stack can be used as arguments to functions. Here is how to manipulate the stack.
`n_swap n` 		Swaps the first item on the stack with the _n-th_.
`n_pop  n` 		Removes _n_ items from the stack.
`clear`	   		Clears the entire stack

### Extending Orava
In your the source code where you embed orava you can use `void compile_builtin(char str[], void(*func)())` to extend
Orava with a new foreign C/C++ function. `str` must be the name of the name function in Orava. `func` a pointer to
a C/C++ function that takes no argument, and returns no value.

TODO: More to follow ...

### Embedding

Simply compile the files with the rest of your code. You can now call the function `eval(const char[])` with any valid _Orava_ code.
