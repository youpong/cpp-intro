# C++ compiler: clang++-10.0.0 not support c++17
#CXX = clang++
CXX = g++

CXXFLAGS =  -std=c++17 -Wall --pedantic-errors

.PHONY: clean format

clean:
	- rm hello all.h.gch
format:
	clang-format -i *.cpp *.h

hello: main.cpp all.h.gch
	$(CXX) -include all.h -o $@ $<
#	$(CXX) $(CXXFLAGS) -include all.h -o $@ $<

all.h.gch: all.h
	$(CXX) -x c++-header -o $@ $^
