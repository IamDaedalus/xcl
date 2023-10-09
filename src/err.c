#include "err.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void err_push(Error **head, char *err_msg, size_t line)
{
	Error *new = NULL;

	if (!head)
	{
		fprintf(stderr, "No head to attach\n");
		return;
	}

	new = malloc(sizeof(Error));
	if (!new)
	{
		fprintf(stderr, "malloc failed at err_push\n");
		exit(EXIT_FAILURE);
	}

	new->err_message = err_msg != NULL ? strdup(err_msg) : NULL;
	new->err_line = line;
	new->next = new->last = NULL;

	if (!*head)
		*head = new;
	else
	{
		/* set last node's next to the new */
		(*head)->last->next = new;
	}

	(*head)->last = new;
}

void err_print(Error *head)
{
	Error *cur = NULL;

	while (head)
	{
		cur = head;
		fprintf(stderr, cur->err_message, cur->err_line);
		head = cur->next;
	}
}

void err_free(Error *head)
{
	Error *cur = NULL;

	while (head)
	{
		cur = head;
		if (cur->err_message)
			free(cur->err_message);
		head = cur->next;
		free(cur);
	}
}
