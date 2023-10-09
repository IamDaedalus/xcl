#ifndef ERR_H
#define ERR_H

#include <stddef.h>

typedef struct Error
{
	char *err_message;
	size_t err_line;
	struct Error *last;
	struct Error *next;
} Error;

/* check if the linked list is empty or not in parsing stage */
void err_push(Error **head, char *err_msg, size_t line);
void err_print(Error *head);
void err_free(Error *head);

#endif /* ERR_H */
