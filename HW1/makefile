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
	$(CC) $(FLAGS) $(TARGET).c lex.h -o $(TARGET).out

ex1:
	./$(TARGET) < ex1.txt
ex2:
	./$(TARGET) < ex2.txt
ex3:
	./$(TARGET) < ex3.txt
test:
	./$(TARGET) < test.txt

.PHONY: clean
clean:
	rm -rf $(TARGET).out