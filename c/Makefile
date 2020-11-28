#CC = clang
#CC = gcc

CFLAGS = -g -Wall -std=c17 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
TARGET = prog

.PHONY: run clean format tags

run: $(TARGET)
	./$(TARGET)
check: $(TARGET)
	./$(TARGET)
clean:
	- rm -f $(TARGET) a.out *.o TAGS
format:
	clang-format -i *.c *.h
tags:
	etags *.c *.h

$(TARGET): main.o util.o
	$(CC) $^ -o $@
main.o: main.c
#	$(CXX) $(CXXFLAGS) -include all.h -c $<
util.o: util.c util.h

