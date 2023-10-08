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
void push_token(Token **head, char *value, size_t line_num, TokenType type)
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
		/* set prev of new to the last node and update
		 * last to the new node
		 */
		new->prev = (*head)->last;
	}

	(*head)->last = new;
}

void free_tokens(Token *head)
{
	Token *cur = NULL;

	if (head)
	{
		while (head)
		{
			cur = head;
			head = cur->next;
			free(cur);
		}
		free(head);
	}
}
