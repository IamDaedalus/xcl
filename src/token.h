#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum TokenType
{
	TOK_START,
	TOK_REGISTER,
	TOK_STRING,
	TOK_NUMBER,
	TOK_END,
} TokenType;

typedef struct Token
{
	char *value;
	size_t line_num;
	TokenType token_type;
	struct Token *last;
	struct Token *next;
	struct Token *prev;
} Token;

void push_token(Token **head, char *value, size_t line_num, TokenType type);
void free_tokens(Token *head);

#endif /* TOKEN_H */
