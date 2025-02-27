#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

typedef enum 
{
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATOR,
    TOKEN_STRING, 
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct 
{
    TokenType type;
    char value[100];
} Token;
Token* getNextToken(FILE *file);
int checkKeyword(const char *str);


#endif
