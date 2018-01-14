# UglyCalc
A simple calculator with lazy evaluation.

## Setting up
### Install
Compile: (Windows)
```
g++ -std=gnu++11 uglycalc.cpp -o uglycalc.exe
```


### Run
Run: (Windows)
```
uglycalc.exe
```
to read from the standard input, or
```
uglycalc.exe < [filepath]
```
if the input is in a file, or
```
uglycalc.exe [filepath]
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

* The problem specification does not ban full numeric register names, like `123`. I assume this is not allowed since it causes ambiguity.
For example the following input:
```
x add 10
10 add 20
x add 10
print x
```
can return 20 or 30 or 40. So, my calculator does not allow full numeric register names.

* My calculator supports integer numbers that fit in `c++ int` data type, which usually corresponds to a 32-bit integers and returns an error message if an input number is out of the range of `int`. It also assumes that all the evaluations whenever `print` is called stay within the `long long` range, otherwise the printed value will be wrong.

* I use `map<string, int> map_values` for memoization, i.e. to avoid computing the value of a register more than once during evaluation. 