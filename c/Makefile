#CC = clang
#CC = gcc

CFLAGS = -g -Wall -std=c17 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
TARGET = prog
SRCS = main.c util.c memcpy.c malloc.c setjmp.c setjmp2.c ptr.c
OBJS = $(SRCS:.c=.o)

.PHONY: run clean check format tags

run: $(TARGET)
	./$(TARGET)
check: $(TARGET)
	./$(TARGET) 
clean:
	- rm -f $(TARGET) *.o a.out *~ TAGS
format:
	clang-format -i $(SRCS) *.h
tags:
	etags $(SRCS) *.h

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

main.o: main.c util.h
util.o: util.c util.h
memcpy.o: memcpy.c util.h
malloc.o: malloc.c util.h
setjmp.o: setjmp.c util.h
setjmp2.o: setjmp2.c util.h
ptr.o: ptr.c util.h
