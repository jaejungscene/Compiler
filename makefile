# the compiler: gcc for C program, define as g++ for C++
CC = gcc
FLEX = flex

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
FLAGS = -ly -lfl

# the build target executable:
TARGET = example
YYLEX = lex.yy.c

all: $(TARGET)

$(TARGET): $(YYLEX) 
	$(CC) $(YYLEX) -o $(TARGET) $(FLAGS)

$(YYLEX): 
	$(FLEX) $(TARGET).l

clean:
	$(RM) -rf $(TARGET) $(YYLEX)



##################################
# wc : 	wc.l wc.y  
# 	bison -d wc.y
# 	flex wc.l
# 	gcc -o $@ wc.tab.c lex.yy.c -ly -lfl

# .PHONY : clean 
# clean : 
# 	rm -rf *.tab.c *.tab.h *.yy.c wc
