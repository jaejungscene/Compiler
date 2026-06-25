## Compiler Study
#### > [About Lex & Yacc](https://wiki.imindlabs.com.au/cs/lang/pl/5_lex_and_yacc/1_basics/)
#### > [Makefile Tutorial](https://makefiletutorial.com/)
```
CC = gcc

target1 : dependency 1 ...
    command 1
```
- special symbols [see here](https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean)
    For example, consider the following declaration:
    ```
    all: library.cpp main.cpp
    ```
    In this case:<br>
    `$@` evaluates to all<br>
    `$<` evaluates to library.cpp<br>
    `$^` evaluates to library.cpp main.cpp<br>
