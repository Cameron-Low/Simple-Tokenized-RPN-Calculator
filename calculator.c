/* 
Calculator program.
Uses Shunting Yard Algorithm to convert from infix to postfix.
Uses RPN evaluation algorithm to evaluate the expression.
Shunting Yard Algorithm - https://en.wikipedia.org/wiki/Shunting-yard_algorithm
Reverse Polish Notation Algorithm - https://en.wikipedia.org/wiki/Reverse_Polish_notation
*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "stack.h"

// Checks if the given character is in the given array.
bool inArray(char c, int arrLen, char arr[]) {
    for (int i = 0; i < arrLen; i++) {
        if (c == arr[i]) {
            return true;
        }
    }
    return false;
}

// Checks if the given character is an operator.
bool isOperator(char c) {
    char operators[] = {'+', '-', '*', '/'};
    int opCount = 4;
    return inArray(c, opCount, operators);
}

// Finds the precedence of the given operator.
int getPrecendence(char op) {
    char opPrecedence[2][2] = {{'+', '-'},{'*','/'}};
    int lenPrecedences = 2;
    for (int i = 0; i < lenPrecedences; i++) {
        if (inArray(op, 2, opPrecedence[i])) {
            return i;
        }
    }
    return -1;
}

// Returns true if x has a lower or equal precedence.
bool comparePrecedence(char x, char y) {
    int xPrec = getPrecendence(x);
    int yPrec = getPrecendence(y);
    return xPrec <= yPrec;
}

// Converts an expression from infix to postfix using the Shunting yard algorithm.
// The ouput string is stored in the result string that was passed in.
void convert(char expr[], char result[]) {
    // Setup operator stack.
    Stack s;
    s.size = strlen(expr);
    s.bp = malloc(s.size * sizeof(Token));
    s.sp = s.bp;

    // Setup output array.
    Token infixExpr[strlen(expr)+1];
    int headPointer = 0;

    // Loop through each character.
    for (int i = 0; i < strlen(expr); i++) {
        if (isdigit(expr[i])) {
            // Create a number token and append it to the array.
            Token tok = {expr[i], (double) (expr[i]-'0')};
            infixExpr[headPointer++] = tok;
        } else if (isOperator(expr[i])){
            // Pop all operators on the stack that are of greater or equal precedence.
            Token topOfStack = peek(&s);
            while ((topOfStack.name != '\0') && (topOfStack.name != '(') && comparePrecedence(expr[i], topOfStack.name)) {
                infixExpr[headPointer++] = pop(&s);
                topOfStack = peek(&s);
            }

            // Push operator to stack.
            Token tok = {expr[i], 0};
            push(&s, tok);
        } else if (expr[i] == '(') {
            // Push to stack to mark end of bracketed expression.
            Token tok = {'(', 0};
            push(&s, tok);
        } else if (expr[i] == ')') {
            // Pop all operators until the leading brace.
            Token topOfStack = peek(&s);
            while (topOfStack.name != '(') {
                infixExpr[headPointer++] = pop(&s);
                topOfStack = peek(&s);
            }
            // Discard the left bracket.
            pop(&s);
        } else {
            // Quit if invalid character.
            printf("%c is not a valid character!\n", expr[i]);
            return;
        }
    }

    // Empty the stack into the output array
    while ((infixExpr[headPointer++] = pop(&s)).name != '\0') {}
    
    // Copy the names of the tokens to the result string.
    for (int i = 0; i < headPointer; i++) {
        if (infixExpr[i].name == '\0') {
            result[i] = '\0';
            break;
        }
        result[i] = infixExpr[i].name;
    }
    free(s.bp);
}

// Evaluates a postfix expression.
double evaluate(char expr[]) {
    // Stack setup.
    Stack s;
    s.size = strlen(expr);
    s.bp = malloc(s.size * sizeof(Token));
    s.sp = s.bp;

    // Loop through expression.
    for (int i = 0; i < strlen(expr); i++) {
        // Evaluate the operation
        if (isOperator(expr[i])) {
            double op2 = pop(&s).value;
            double op1 = pop(&s).value;

            Token tok;
            tok.name = 'r';
            switch (expr[i]) {
                case '+': tok.value = op1 + op2; break;
                case '-': tok.value = op1 - op2; break;
                case '*': tok.value = op1 * op2; break;
                case '/': tok.value = op1 / op2; break;
            }

            // Push the resulting value back to the stack.
            push(&s,tok);
        } else {
            // Push the value onto the stack.
            Token tok;
            tok.name = expr[i];
            tok.value = (double) (expr[i]-'0');
            push(&s, tok);
        }
    }

    // Return the value of the expression.
    double result = pop(&s).value;
    free(s.bp);
    return result;
}

// Testing the convert function.
void testConvert() {
    char result[25];
    convert("2+2", result);
    assert(!strcmp(result,"22+"));
    convert("2+2-3+3-2", result);
    assert(!strcmp(result,"22+3-3+2-"));
    convert("2+2*4/8-2", result);
    assert(!strcmp(result, "224*8/+2-"));
    convert("2/(2+2)*(2-3+2)+3-2", result);
    assert(!strcmp(result, "222+/23-2+*3+2-"));
}

// Testing the evaluate function.
void testEvaluate() {
    assert(evaluate("22+4+2+3+") == 13);
    assert(evaluate("22+3-3+2-") == 2);
    assert(evaluate("224*8/+2-") == 1);
    assert(evaluate("222+/23-2+*3+2-") == 1.5);
}

// Run all tests.
void test() {
    testConvert();
    testEvaluate();
    printf("All tests passed!\n");
}

// Run with ./RPN "{expr}" for a custom expression. No arguments will test.
int main(int argc, char *args[argc]) {
    setbuf(stdout, NULL);
    if (argc == 1) {
        test();
    } else if (argc == 2) {
        char result[strlen(args[1])+1];
        convert(args[1], result);
        printf("%f\n", evaluate(result));
    } else {
        fprintf(stderr, "Use e.g.: ./calc expression\n");
        exit(1);
    }
    return 0;
}
