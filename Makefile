TARGET = prog
SRCS = main.cpp error.cpp reference.cpp
OBJS = $(SRCS:.cpp=.o)

# C++ compiler
# use clang++ for ubuntu-latest(18.04) C/C++ CI
CXX = clang++
# CXX = g++

# c++17 c++2a environment
# okay  okay  github/ubuntu-18.04/clang++
# okay  not   github/ubuntu-18.04/g++
# okay  okay  ubuntu-20.04/clang++10.0.0-4
# okay  okay  ubuntu-20.04/g++-9.3.0-17
#CXXFLAGS =  -std=c++17 -Wall --pedantic-errors
CXXFLAGS =  -std=c++2a -Wall --pedantic-errors

.PHONY: run clean check format tags cloc list

list:
	@sed -n 's/.PHONY:[[:space:]]*//p' Makefile
run: $(TARGET)
	./$(TARGET)
check: $(TARGET)
	./$(TARGET)
clean:
	- rm -f $(TARGET) all.h.gch a.out *.o *~ TAGS
format:
	clang-format -i $(SRCS) *.h
tags:
	etags *.cpp *.h
cloc:
	cloc $(SRCS) *.h

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@
main.o: main.cpp all.h all.h.gch
	$(CXX) $(CXXFLAGS) -include all.h -c $<
error.o: error.cpp all.h all.h.gch
	$(CXX) $(CXXFLAGS) -include all.h -c $<
reference.o: reference.cpp all.h all.h.gch
	$(CXX) $(CXXFLAGS) -include all.h -c $<
all.h.gch: all.h
	$(CXX) $(CXXFLAGS) -x c++-header -o $@ $<
