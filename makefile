run:
	clang -std=c11 -Wall -pedantic -g calculator.c stack.c lexer.c -o calc -fsanitize=undefined -fsanitize=address

lex:
	clang -std=c11 -Wall -pedantic -g lexer.c -o lex -fsanitize=undefined -fsanitize=address
