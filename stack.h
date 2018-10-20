/*Header file for stack.c*/

// A token
typedef struct {
    char name;
    double value;
} Token;

// A token stack.
typedef struct {
    Token *bp; // Base pointer (bottom of the stack)
    Token *sp; // Stack pointer (current element)
    int size; // Size of the stack
} Stack;

Token peek(Stack* stack);

Token pop(Stack* stack);

void push(Stack* stack, Token object);

