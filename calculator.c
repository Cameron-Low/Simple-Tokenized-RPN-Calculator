/* 
Calculator program.
Uses Shunting Yard Algorithm to convert from infix to postfix.
Uses RPN evaluation algorithm to evaluate the expression.
Shunting Yard Algorithm - https://en.wikipedia.org/wiki/Shunting-yard_algorithm
Reverse Polish Notation Algorithm - https://en.wikipedia.org/wiki/Reverse_Polish_notation
*/
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

// Private function signatures.
int getPrecendence(char op);
int comparePrecedence(char x, char y);
int convert(Token expr[], Token result[]);
double evaluate(Token expr[]);
void testConvert();
void testEvaluate();
void test();

// Converts an expression from infix to postfix using the Shunting yard algorithm.
// The ouput string is stored in the result string that was passed in.
int convert(Token expr[], Token result[]) {
    // Setup operator stack.
    Stack s;
    s.size = tokLength(expr);
    s.bp = malloc(s.size * sizeof(Token));
    s.sp = s.bp;

    // Setup output array.
    Token infixExpr[tokLength(expr)+1];
    int headPointer = 0;

    // Loop through each character.
    for (int i = 0; i < tokLength(expr); i++) {
        if (expr[i].name == 'n') {
            infixExpr[headPointer++] = expr[i];
        } else if (isOperator(expr[i].name)){
            // Pop all operators on the stack that are of greater or equal precedence.
            Token topOfStack = peek(&s);
            while ((topOfStack.name != '\0') && (topOfStack.name != '(') && ((comparePrecedence(expr[i].name, topOfStack.name) == 0 && topOfStack.name != '^') || comparePrecedence(expr[i].name, topOfStack.name) < 0)) {
                infixExpr[headPointer++] = pop(&s);
                topOfStack = peek(&s);
            }

            push(&s, expr[i]);
        } else if (expr[i].name == '(') {
            push(&s, expr[i]);
        } else if (expr[i].name == ')') {
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
            printf("%c is not a valid character!\n", expr[i].name);
            return -1;
        }
    }

    // Empty the stack into the output array
    while ((infixExpr[headPointer++] = pop(&s)).name != '\0') {}
    
    // Copy the names of the tokens to the result string.
    for (int i = 0; i < headPointer; i++) {
        result[i] = infixExpr[i];
    }
    free(s.bp);
    return 0;
}

// Evaluates a postfix expression.
double evaluate(Token expr[]) {
    // Stack setup.
    Stack s;
    s.size = tokLength(expr);
    s.bp = malloc(s.size * sizeof(Token));
    s.sp = s.bp;

    // Loop through expression.
    for (int i = 0; i < tokLength(expr); i++) {
        // Evaluate the operation
        if (isOperator(expr[i].name)) {
            double op2 = pop(&s).value;
            double op1 = pop(&s).value;

            Token tok = {'n', 0};
            switch (expr[i].name) {
                case '+': tok.value = op1 + op2; break;
                case '-': tok.value = op1 - op2; break;
                case '*': tok.value = op1 * op2; break;
                case '/': tok.value = op1 / op2; break;
                case '^': tok.value = pow(op1, op2); break;
            }

            // Push the resulting value back to the stack.
            push(&s,tok);
        } else {
            // Push the value onto the stack.
            push(&s, expr[i]);
        }
    }

    // Return the value of the expression.
    double result = pop(&s).value;
    free(s.bp);
    return result;
}

// _______________________________
//           Helpers
// _______________________________

// Finds the precedence of the given operator.
int getPrecendence(char op) {
    char opPrecedence[3][2] = {{'+', '-'},{'*','/'},{'^','^'}};
    int lenPrecedences = 3;
    for (int i = 0; i < lenPrecedences; i++) {
        if (inArray(op, 2, opPrecedence[i])) {
            return i;
        }
    }
    return -1;
}

// Returns true if x has a lower or equal precedence.
int comparePrecedence(char x, char y) {
    int xPrec = getPrecendence(x);
    int yPrec = getPrecendence(y);
    return xPrec - yPrec;
}

// _______________________________
//           Testing
// _______________________________

// Testing the convert function.
void testConvert() {
    Token toks[30];
    lex(toks, "3+3*(3-1)");
    convert(toks, toks);

    Token check[] = {createToken('n', 3), createToken('n', 3), createToken('n', 3), createToken('n', 1),
                    createToken('-', 0), createToken('*', 0), createToken('+', 0)};
    assert(tokensMatch(toks, check));
}

// Testing the evaluate function.
void testEvaluate() {
    Token check[] = {createToken('n', 3), createToken('n', 3), createToken('n', 3), createToken('n', 1),
                    createToken('-', 0), createToken('*', 0), createToken('+', 0), createToken('\0', 0)};
    assert(evaluate(check) == 9);
}

// Run all tests.
void test() {
    testNumberHandling();
    testLexer();
    testConvert();
    testEvaluate();
    printf("All tests passed!\n");
}

// _______________________________
//           Running
// _______________________________

// Run with ./calc "{expr}" for a custom expression. No arguments will test.
int main(int argc, char *args[argc]) {
    setbuf(stdout, NULL);
    if (argc == 1) {
        test();
    } else if (argc == 2) {
        Token result[strlen(args[1])+1];
        if (lex(result, args[1])) return 1;
        if (convert(result, result)) return 1;
        printf("%f\n", evaluate(result));
    } else {
        fprintf(stderr, "Use e.g.: ./calc expression\n");
        exit(1);
    }
    return 0;
}
