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


TEST01 = test01
$(TEST01):
	flex $(TEST01).l
	gcc $(YYLEX) -o $(TEST01) $(FLAGS)
	rm $(YYLEX)

MY = my
$(MY):
	flex $(MY).l
	gcc $(YYLEX) -o $(MY) $(FLAGS)
	rm $(YYLEX)

clean:
	$(RM) -rf $(TARGET) $(YYLEX) $(TEST01) $(MY)



##################################
# wc : 	wc.l wc.y  
# 	bison -d wc.y
# 	flex wc.l
# 	gcc -o $@ wc.tab.c lex.yy.c -ly -lfl

# .PHONY : clean 
# clean : 
# 	rm -rf *.tab.c *.tab.h *.yy.c wc
