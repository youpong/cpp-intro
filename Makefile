.PHONY: clean

CPPFLAGS = -std=c++17 -Wall --pedantic-errors

clean:
	- rm hello
format:
	clang-format -i *.cpp
