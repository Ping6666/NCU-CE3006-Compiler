# make file for yacc, lex

##### exe file base name #####
result = filename
yacc_in = $(result).y
lex_in = $(result).l

# yacc middle file name
# yacc = y.tab
yacc = $(result).tab

# lex middle file name
lex = lex.yy

##### make all #####
all: $(result)

$(yacc).o: $(yacc_in)
	bison -d -o $(yacc).c $(yacc_in)
	gcc -c -g -I.. $(yacc).c

$(lex).o: $(lex_in)
	flex -o $(lex).c $(lex_in)
	gcc -c -g -I.. $(lex).c

$(result): $(yacc).o $(lex).o
	gcc -o $(result) $(lex).o $(yacc).o -ll
	rm -f $(yacc).* $(lex).*

##### make clean #####
clean:
	rm -f $(result) $(yacc).* $(lex).*
