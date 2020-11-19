# C++ compiler: clang++-10.0.0 not support c++17
#CXX = clang++
CXX = g++

CXXFLAGS =  -std=c++17 -Wall --pedantic-errors

.PHONY: run clean format tags

run: hello
	./hello
check: hello
	./hello
clean:
	- rm hello all.h.gch
format:
	clang-format -i *.cpp *.h
tags:
	etags *.cpp *.h

hello: main.cpp all.h all.h.gch
	$(CXX) $(CXXFLAGS) -include all.h $< -o $@
#	$(CXX) -include all.h $< -o $@

all.h.gch: all.h
	$(CXX) $(CXXFLAGS) -x c++-header -o $@ $<
