/*
Lexer program.
This generates tokens that represent numbers/operators/characters.
Has basic error checking: multiple decimal points, invalid characters.
*/
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

// Private function signatures
int handleNumber(char expr[], int numStart, char number[]);

// Convert a string into an array of tokens, ignoring spaces.
int lex(Token tokens[], char expr[]) {
    int tokIndex = 0;
    int skip = 0;
    for (int i = 0; i < strlen(expr); i++) {
        // Skip through characters after dealing with a number.
        if (skip > 0) {
            skip--;
            continue;
        }

        char currentChar = expr[i];
        if (currentChar == ' ') {
            // Ignore spaces.
            continue;
        } else if (currentChar == '(') {
            tokens[tokIndex] = createToken('(', 0);
        } else if (currentChar == ')') {
            tokens[tokIndex] = createToken(')', 0);
        } else if (isdigit(currentChar)){
            char number[strlen(expr)];
            skip = handleNumber(expr, i, number);
            if (skip < 0) return -1;
            tokens[tokIndex] = createToken('n', atof(number));
        } else if (isOperator(expr[i])) {
            tokens[tokIndex] = createToken(expr[i], 0);
        } else {
            printf("Unkown character {%c}!\nFound at position %d in the expression.\n", expr[i], i);
            return -1;
        }
        tokIndex++;
    }
    // Add the null token.
    tokens[tokIndex] = createToken('\0', 0);
    return 0;
}

// _______________________________
//           Helpers
// _______________________________

// Creates a token with the supplied name and value.
Token createToken(char name, double value) {
    Token tok = {name, value};
    return tok;
}

// Extracts the number from the expression.
int handleNumber(char expr[], int numStart, char number[]) {
    int exprIndex = numStart;
    int numIndex = 0;
    bool hitDot = false;
    while (isdigit(expr[exprIndex]) || expr[exprIndex] == '.') {
        // Check for multiple decimal points.
        if (expr[exprIndex] == '.') {
            if (hitDot) {
                printf("A number cannot have two decimal points!\nFound at position %d in the expression.\n", exprIndex+1);
                return -1;
            } else {
                hitDot = true;
            }
        }

        number[numIndex] = expr[exprIndex];
        numIndex++;
        exprIndex++;
    }
    number[numIndex] = '\0';
    return numIndex-1;
}

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
    char operators[] = {'+', '-', '*', '/', '^'};
    int opCount = 5;
    return inArray(c, opCount, operators);
}

// Print out the name and value of all tokens in an array.
void printTokens(Token toks[]) {
    for (int i = 0; i < tokLength(toks); i++) {
        printf("Token\t%c\t%f\n", toks[i].name, toks[i].value);
    }
}

// Check two token arrays match exactly. Returns true if this is the case.
bool tokensMatch(Token tok1[], Token tok2[]) {
    int index = 0;
    while (tok1[index].name != '\0' && tok2[index].name != '\0') {
        if (tok1[index].name == tok2[index].name && tok1[index].value == tok2[index].value) {
            index++;
        } else {
            return false;
        }
    }
    return true;
}

// Returns length of a token string.
int tokLength(Token toks[]) {
    int index = 0;
    while (toks[index].name != '\0') {
        index++;
    }
    return index;
}

// _______________________________
//           Testing
// _______________________________

// Check that the lexer can extract all types of numbers.
void testNumberHandling() {
    char expr[] = "1.2/7+8+34+44.4232";
    char number[20];
    handleNumber(expr, 0, number);
    assert(!strcmp(number, "1.2"));
    handleNumber(expr, 4, number);
    assert(!strcmp(number, "7"));
    handleNumber(expr, 6, number);
    assert(!strcmp(number, "8"));
    handleNumber(expr, 8, number);
    assert(!strcmp(number, "34"));
    handleNumber(expr, 11, number);
    assert(!strcmp(number, "44.4232"));
}

// Test the output of tokens from the lexer is what is expected.
void testLexer() {
    Token tokens[20];
    lex(tokens, "(2+2)/5*65.4");
    Token expected[] = {createToken('(', 0), createToken('n', 2), createToken('+', 0), createToken('n', 2), createToken(')', 0),
                        createToken('/', 0), createToken('n', 5), createToken('*', 0), createToken('n', 65.4), createToken('\0', 0)};
    assert(tokensMatch(expected, tokens));
}
