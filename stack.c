/* 
This is a library for a Token stack.
It contains useful functions for manipulating a Token stack.
It also contains the definition of a Token.
*/
#include "stack.h"

// Return the token at the top of the stack without changing the stack pointer.
Token peek(Stack* stack) {
    Token t = {'\0', 0};
    if (stack->sp == stack->bp) return t;
    return *(stack->sp);
}

// Returns the token at the top of the stack and decrements the stack pointer.
Token pop(Stack* stack) {
    Token object = *(stack->sp); // Get the token at the top of the stack

    if (stack->sp == stack->bp) {
        stack->sp = stack->bp;
        Token t = {'\0', 0};
        return t;// Caller should check for this
    }
    (stack->sp)--;
    return object;
}

// Add token to the stack.
void push(Stack* stack, Token object) {
    (stack->sp)++;
    if (stack->sp > (stack->bp + stack->size)) {
        (stack->sp)--;
        return;
    }
    *(stack->sp) = object;
}
