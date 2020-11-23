# CXX: C++ compiler
#   clang++-10.0.0-4ubuntu1: not support c++17
#CXX = clang++
CXX = g++

CXXFLAGS =  -std=c++17 -Wall --pedantic-errors

.PHONY: run clean format tags

run: hello
	./hello
check: hello
	./hello
clean:
	- rm -f hello a.out all.h.gch *.o TAGS
format:
	clang-format -i *.cpp *.h
tags:
	etags *.cpp *.h

hello: main.o error.o
	$(CXX) $^ -o $@
main.o: main.cpp all.h all.h.gch
	$(CXX) $(CXXFLAGS) -include all.h -c $<
error.o: error.cpp all.h all.h.gch
	$(CXX) $(CXXFLAGS) -include all.h -c $<
all.h.gch: all.h
	$(CXX) $(CXXFLAGS) -x c++-header -o $@ $<
