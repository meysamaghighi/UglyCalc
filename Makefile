all: calc.cpp
	g++ -std=gnu++11 calc.cpp -o calc.out

test:
	./calc.out input/a.in
	./calc.out input/b.in
	./calc.out input/c.in
	./calc.out input/d.in