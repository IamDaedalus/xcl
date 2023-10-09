#include "lexer.h"
#include "helpers.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void lex_begin(char *file_buf, size_t size, Token **head)
{
	size_t line = 1;
	size_t i = 0;

	(void)head;

	while (i < size)
	{
		char c = file_buf[i];

		if (c == '\n')
		{
			token_push(head, NULL, line, TOK_NEWLINE);
			line++;
		}
		else if (c == '#')
		{
			while (file_buf[i] != '\n')
				i++;
		}
		else if (is_alpha(c) != 0)
		{
			int start = i;

			/* extract substr of the keyword */
			while ((is_alpha(file_buf[i]) != 0))
				i++;

			int kw_len = i - start;
			char *kw = malloc(sizeof(char) * kw_len + 1);
			if (!kw)
			{
				fprintf(stderr, "malloc failed at lex_begin\n");
				exit(EXIT_FAILURE);
			}

			strncpy(kw, &file_buf[start], kw_len);
			kw[kw_len] = '\0';

			token_push(head, kw, line, check_identifier(kw));

			free(kw);
			i--;
		}
		i++;
	}
}

TokenType check_identifier(char *identifier)
{
	int codes_len = 0, reg_len = 0;
	int i = 0;

	char *opcodes[] = {
		"mov", "add", "mul", "sub", "div", "mod",
		"push", "pop", "swap", 
	};

	char *registers[] = {
		"cx", "reg1", "reg2", "reg3", "reg0",
	};

	codes_len = sizeof(opcodes) / sizeof(opcodes[0]);
	reg_len = sizeof(registers) / sizeof(registers[0]);

	/* check the identifier against known opcodes */
	while (i < codes_len)
	{
		if (strncmp(opcodes[0], identifier, strlen(identifier)) == 0)
			return TOK_OPCODE;

		i++;
	}
	i = 0; /* reset i for registers check */

	/* check the identifier against known registers */
	while (i < reg_len)
	{
		if (strncmp(registers[0], identifier, strlen(identifier)) == 0)
			return TOK_REGISTER;

		i++;
	}

	/* identifier is not a valid value */
	return TOK_UNKNOWN;
}
