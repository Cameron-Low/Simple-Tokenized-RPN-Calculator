/* Header file for lexer.c */
#include <stdbool.h>

// Token type
typedef struct {
    char name;
    double value;
} Token;

// Public function signatures
bool inArray(char c, int arrLen, char arr[]);
bool isOperator(char op);
int lex(Token tokens[], char expr[]);
void printTokens(Token toks[]);
bool tokensMatch(Token tok1[], Token tok2[]);
int tokLength(Token toks[]);
Token createToken(char name, double value);
void testNumberHandling();
void testLexer();
