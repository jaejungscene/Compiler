# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
FLAGS = -ly -lfl

# the build target executable:
TARGET = calc
YYLEX = lex.yy.c
# OUT = parser_201923674.out
OUT = calc

all: clean $(TARGET)

$(TARGET):
	bison $(TARGET).y
	gcc -o $(OUT) $(TARGET).tab.c

clean:
	$(RM) -rf $(TARGET).tab.c $(OUT)



##################################
# wc : 	wc.l wc.y  
# 	bison -d wc.y
# 	flex wc.l
# 	gcc -o $@ wc.tab.c lex.yy.c -ly -lfl

# .PHONY : clean 
# clean : 
# 	rm -rf *.tab.c *.tab.h *.yy.c wc
