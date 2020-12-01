TARGET = prog
SRCS = main.cpp error.cpp reference.cpp
OBJS = $(SRCS:.cpp=.o)

# C++ compiler
#   clang++-10.0.0-4ubuntu1: not support c++17
# CXX = clang++
# CXX = g++

CXXFLAGS =  -std=c++17 -Wall --pedantic-errors

.PHONY: run clean format tags

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
