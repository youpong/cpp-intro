#CXX = g++
CXX = clang++
CPPFLAGS = -std=c++17 -Wall --pedantic-errors

.PHONY: clean format

clean:
	- rm hello
format:
	clang-format -i *.cpp *.h
