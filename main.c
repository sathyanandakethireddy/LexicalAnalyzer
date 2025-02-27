#include "main.h"

FILE *file;
//int line = 1, col = 1;

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) 
    {
        perror("Error opening file");
        return 1;
    }

    Token *token;
    char type[50];
    
    while ((token = getNextToken(fp)) != NULL && token->type != TOKEN_EOF)
     {
        if (token->type != TOKEN_ERROR) 
        {
            memset(type, 0, sizeof(type));
            switch (token->type) {
                case TOKEN_KEYWORD:
                    strcpy(type, "Keyword");
                    break;
                case TOKEN_IDENTIFIER:
                    strcpy(type, "Identifier");
                    break;
                case TOKEN_NUMBER:
                    strcpy(type, "Number");
                    break;
                case TOKEN_OPERATOR:
                    strcpy(type, "Operator");
                    break;
                case TOKEN_PUNCTUATOR:
                    strcpy(type, "Punctuator");
                    break;
                case TOKEN_STRING:
                    strcpy(type, "String literal");
                    break;
                default:
                    strcpy(type, "Unknown");
                    break;
            }
            printf("Type: %s:      %s\n", type, token->value);
        }
        free(token);
    }
    
    fclose(fp);
    return 0;
}
