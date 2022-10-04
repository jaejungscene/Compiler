# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = hw1_201923674

all: clean $(TARGET)

$(TARGET):
	$(CC) $(FLAGS) lexical-analyzer.c -o $(TARGET).exe

ex1:
	./$(TARGET).exe < ex1.txt > ex1-result.txt
	./$(TARGET).exe < ex1.txt
ex2:
	./$(TARGET).exe < ex2.txt > ex2-result.txt
	./$(TARGET).exe < ex2.txt
ex3:
	./$(TARGET).exe < ex3.txt > ex3-result.txt
	./$(TARGET).exe < ex3.txt
test:
	./$(TARGET).exe < test.txt

.PHONY: clean
clean:
	rm -rf $(TARGET).exe