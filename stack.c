/* This is a library of basic stack functions like pop, push, etc.*/
#include <stdio.h>
#include <stdlib.h>

// A character stack.
typedef struct {
    char *bp; // Base pointer (bottom of the stack)
    char *sp; // Stack pointer (current element)
    int size; // Size of the stack
} Stack;


// Returns the character at the top of the stack and decrements the stack pointer.
char pop(Stack* stack) {
    char object = *(stack->sp); // Get the object at the top of the stack

    if (stack->sp == stack->bp) {
        printf("Stack is empty!\n");
        stack->sp = stack->bp;
        return '\0'; // Caller should check for this
    }
    (stack->sp)--;
    return object;
}

// Add element to the stack.
void push(Stack* stack, char object) {
    (stack->sp)++;
    if (stack->sp > (stack->bp + stack->size)) {
        (stack->sp)--;
        printf("Stack is full!\n");
        return;
    }
    *(stack->sp) = object;
}

int main() {
    Stack s;
    s.size = 1;
    s.bp = malloc(sizeof(char)*s.size);
    s.sp = s.bp;
    printf("%p\n", (s.bp));
    push(&s, 'C');
    push(&s, 'D');
    char c = pop(&s);
    printf("The stack had a %c\n", c);
    c = pop(&s);
    printf("The stack had a %c\n", c);
    c = pop(&s);
    free(s.bp);
}
