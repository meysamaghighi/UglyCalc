# UglyCalc
A simple calculator with lazy evaluation.

## Setting up
### Install
Compile: (Windows)
```
g++ -std=gnu++11 calc.cpp -o calc.exe
```


### Run
Run: (Windows)
```
calc.exe
```
to read from the standard input, or
```
calc.exe < [filename]
```
if the input is in a file, or
```
calc.exe [filename]
```
to read from a file.

## Assumptions & Limitations
* If there are cycles in the dependecy graph of the input variables, then there can be contradictions. For example:
```
a add b
b add 10
b add a
print a
```
will result in a = 10 + a, a contradiction. So, my calculator throws an exception whenever there is a cycle in the dependency graph of the input variables. Even the case of a self-cycle like `a add a`.

* The problem specification allows full numeric variable names, like `123`. We assume this is not allowed since it causes ambiguity.
For example the following input:
```
x add 10
10 add 20
x add 10
print x
```
can return 20 or 30 or 40. I prioritize numbers in my code and will return `20` for the above example.

* Another example is the following input:
```
10 add 20
print 10
```
for which my calculator returns 10 not 20.

* My calculator only supports 32-bit integers and assumes that all the input numbers (and their evaluations whenever `print` is called) are within the 32-bit integer, otherwise we get `out_of_range` exception.

* I use `map<string, int> map_values` for memoization, i.e. to avoid computing the value of a register more than once during evaluation. 