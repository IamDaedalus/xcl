#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * push_token - pushes a new token to the linked list
 * @head: the head of the linked list token
 * @value: the value of the token
 * @line_num: the line number the token is coming from
 * @type: the type of token; useful for the parser in later stages
 */
void token_push(Token **head, char *value, size_t line_num, TokenType type)
{
	Token *new = NULL;
	if (!head)
	{
		fprintf(stderr, "No linked list to push to\n");
		return;
	}

	new = malloc(sizeof(Token));
	if (!new)
	{
		fprintf(stderr, "Malloc failed in push_token\n");
		exit(EXIT_FAILURE);
	}

	new->value = value != NULL ? strdup(value) : NULL;
	new->line_num = line_num;
	new->token_type = type;
	new->last = new->next = new->prev = NULL;

	if (!(*head))
		*head = new;
	else
	{
		/* set last as new node's prev and set
		 * last node's next as new node
		 */
		new->prev = (*head)->last;
		(*head)->last->next = new;
	}

	/* update the last node to the new node */
	(*head)->last = new;
}

void token_print(Token *head)
{
}

void token_free(Token *head)
{
	Token *cur = NULL;

	while (head)
	{
		cur = head;
		if (cur->value)
			free(cur->value);
		head = cur->next;
		free(cur);
	}
}

/**
 * token_str - gets the string format of the enum
 * @tok: the token type
 * Return: returns the string format
 */
char *token_str(TokenType tok)
{
	switch(tok)
	{
		case TOK_NUMBER:
			return "[number]";
			break;
		case TOK_NEWLINE:
			return "[newline]";
			break;
		case TOK_START:
			return "[start]";
			break;
		case TOK_END:
			return "[end]";
			break;
		case TOK_STRING:
			return "[string]";
			break;
		case TOK_REGISTER:
			return "[register]";
			break;
		case TOK_OPCODE:
			return "[opcode]";
			break;
		case TOK_UNKNOWN:
			return "[unknown]";
			break;
	}

	return NULL;
}
