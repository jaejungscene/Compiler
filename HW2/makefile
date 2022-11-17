# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
FLAGS = -ly -lfl

# the build target executable:
TARGET = interpreter
YYLEX = lex.yy.c
OUT = parser_201923674.out

all: clean $(TARGET)

$(TARGET):
	flex $(TARGET).l
	gcc $(YYLEX) -o $(OUT) $(FLAGS)
	rm $(YYLEX)

clean:
	$(RM) -rf $(TARGET) $(YYLEX) $(OUT)



##################################
# wc : 	wc.l wc.y  
# 	bison -d wc.y
# 	flex wc.l
# 	gcc -o $@ wc.tab.c lex.yy.c -ly -lfl

# .PHONY : clean 
# clean : 
# 	rm -rf *.tab.c *.tab.h *.yy.c wc
