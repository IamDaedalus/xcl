#include "lexer.h"
#include <stdio.h>
#include <stddef.h>

void lex_begin(char *file_buf, size_t size, Token **head)
{
	(void)head;

	printf("%s %lu\n", file_buf, size);
}
