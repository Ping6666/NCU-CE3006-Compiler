##### exe file base name #####
result = Mini_LISP
yacc_in = $(result).y
lex_in = $(result).l

##### compile language #####
CC = g++

# ast associate file
ast = ast

# yacc middle file name
# yacc = y.tab
yacc = $(result).tab

# lex middle file name
lex = lex.yy

##### make all #####
all: $(result)

$(ast).o: $(ast).cpp
	$(CC) -c $(ast).cpp

$(yacc).o: $(yacc_in)
	bison -d -o $(yacc).c $(yacc_in)
	$(CC) -c -g -I.. $(yacc).c

$(lex).o: $(lex_in)
	flex -o $(lex).c $(lex_in)
	$(CC) -c -g -I.. $(lex).c

$(result): $(ast).o $(yacc).o $(lex).o
	$(CC) -o $(result) $(ast).o $(yacc).o $(lex).o -ll
	rm -f $(ast).o $(yacc).* $(lex).*

##### make clean #####
clean:
	rm -f $(result) $(ast).o $(yacc).* $(lex).*
