/*Header file for stack.c*/
#include "lexer.h"

// A token stack.
typedef struct {
    Token *bp; // Base pointer (bottom of the stack)
    Token *sp; // Stack pointer (current element)
    int size; // Size of the stack
} Stack;

// Public function signatures.
Token peek(Stack* stack);
Token pop(Stack* stack);
void push(Stack* stack, Token object);
