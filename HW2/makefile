# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
FLAGS = -ly -lfl

# the build target executable:
TARGET = interpreter
YYLEX = lex.yy.c

all: clean $(TARGET)

$(TARGET): $(YYLEX) 
	gcc $(YYLEX) -o $(TARGET) $(FLAGS)
	rm $(YYLEX)

$(YYLEX): 
	flex $(TARGET).l


test02:
	flex test02.l
	gcc $(YYLEX) -o test02 $(FLAGS)
	rm $(YYLEX)

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
