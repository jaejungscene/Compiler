# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = lexical-analyzer

all: clean $(TARGET)

$(TARGET):
	$(CC) $(FLAGS) $(TARGET).c -o $(TARGET)

ex1:
	./$(TARGET) < ex1.txt

.PHONY: clean
clean:
	rm -rf $(TARGET)