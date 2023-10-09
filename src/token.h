#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum TokenType
{
	TOK_START,
	TOK_NEWLINE,
	TOK_REGISTER,
	TOK_OPCODE,
	TOK_STRING,
	TOK_NUMBER,
	TOK_END,
	TOK_UNKNOWN,
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

void token_push(Token **head, char *value, size_t line_num, TokenType type);
void token_free(Token *head);
void token_print(Token *head);
char *token_str(TokenType tok);

#endif /* TOKEN_H */
