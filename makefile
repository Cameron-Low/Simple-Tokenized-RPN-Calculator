run:
	clang -std=c11 -Wall -pedantic -g calculator.c stack.c -o calc -fsanitize=undefined -fsanitize=address
