#include "main.h"

//FILE *file;
int line = 1, col = 1;

const char *keywords[] = {"int","float","return","if","else","while","for","do","break","continue",
    "char","double","void","switch","case","default","const","static","sizeof","struct","volatile","long"};
int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

int is_keyword(const char *str) 
{
    for (int i = 0; i < num_keywords; i++) 
    {
        if (strcmp(str, keywords[i]) == 0) 
        {
            return 1;
        }
    }
    return 0;
}

Token* getNextToken(FILE *file) 
{
    char token[100] = {0};
    int tokenLength = 0;
    int string_literal = 0, string_lite = 0;
    char c;
    int singleline = 0, multiline = 0;
    while ((c = fgetc(file)) != EOF) 
    {
        col++;
        if (c == '\n') 
        {
            line++;
            col = 1;
        }
        if (string_literal) 
        {
            if (c == '"') 
            {
                string_literal = 0;
                string_lite = 1;
                break;
            } 
            else 
            {
                token[tokenLength++] = c;
            }
            continue;
        }
        if (c == '"') {
            string_literal = 1;
            continue;
        }
        if (singleline) {
            if (c == '\n') {
                singleline = 0;
            }
            continue;
        }
        if (multiline) {
            if (c == '*') {
                char next_c = fgetc(file);
                if (next_c == '/') {
                    multiline = 0;
                }
            }
            continue;
        }
        if (isspace(c)) {
            if (tokenLength > 0) {
                break;
            } else {
                continue;
            }
        }
        if (c == '/') {
            char next_c = fgetc(file);
            if (next_c == '/') {
                singleline = 1;
                continue;
            } else if (next_c == '*') {
                multiline = 1;
                continue;
            }
        }
        if (strchr("+-*/=<>!&|", c)) {
            if (tokenLength > 0) {
                ungetc(c, file);
                break;
            } else {
                token[tokenLength++] = c;
                char next_ch = fgetc(file);
                if (next_ch == '=') {
                    token[tokenLength++] = next_ch;
                } else {
                    ungetc(next_ch, file);
                }
                break;
            }
        }
        if (strchr("(){}[],:;", c)) {
            if (tokenLength > 0) {
                ungetc(c, file);
                break;
            } else {
                token[tokenLength++] = c;
                break;
            }
        }
        if (isalnum(c) || c == '_') {
            token[tokenLength++] = c;
        } else {
            fprintf(stderr, "Error: Invalid character '%c' at line %d, column %d\n", c, line, col);
            Token *newToken = (Token *)malloc(sizeof(Token));
            newToken->type = TOKEN_ERROR;
            strcpy(newToken->value, "");
            return newToken;
        }
        if (tokenLength >= 100 - 1) {
            fprintf(stderr, "Error: Token too long at line %d, column %d\n", line, col);
            Token *newToken = (Token *)malloc(sizeof(Token));
            newToken->type = TOKEN_ERROR;
            strcpy(newToken->value, "");
            return newToken;
        }
    }
    if (tokenLength > 0) token[tokenLength] = '\0';
    Token *newToken = (Token *)malloc(sizeof(Token));
    if (c == EOF) {
        newToken->type = TOKEN_EOF;
        strcpy(newToken->value, "");
        return newToken;
    }
    strcpy(newToken->value, token);
    if (is_keyword(token) && !string_lite) {
        newToken->type = TOKEN_KEYWORD;
    } else if ((isalpha(token[0]) || token[0] == '_') && !string_lite) {
        newToken->type = TOKEN_IDENTIFIER;
    } else if (isdigit(token[0])) {
        newToken->type = TOKEN_NUMBER;
    } else if (strchr("+-*/=<>!&|", token[0])) {
        newToken->type = TOKEN_OPERATOR;
    } else if (strchr("(){}[],:;", token[0])) {
        newToken->type = TOKEN_PUNCTUATOR;
    } else if (string_lite) {
        newToken->type = TOKEN_STRING;
    } else {
        newToken->type = TOKEN_ERROR;
    }
    return newToken;
}

