# make file for lex

result = filename
lex_in = $(result).l
lexout = lex.yy.c

all: $(result)

$(lexout): $(lex_in)
	flex -o $(lexout) $(lex_in)

$(result): $(lexout)
	gcc -o $(result) $(lexout) -lfl

clean:
	rm -f $(result) $(lexout)
