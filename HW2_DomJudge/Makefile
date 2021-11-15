result = LEX_A_CodingStyle
lexout = lex.yy.c
lexin = LEX_A_CodingStyle.l

all: $(result)

$(lexout): $(lexin)
	flex -o $(lexout) $(lexin)

$(result): $(lexout)
	gcc -o $(result) $(lexout) -lfl

clean:
	rm -f $(result) $(lexout)
