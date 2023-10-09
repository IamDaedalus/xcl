#include "lexer.h"
#include "err.h"
#include "helpers.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void lex_begin(char *file_buf, size_t size, Token **tokens, Error **err)
{
	size_t line = 1;
	size_t i = 0;

	while (i < size)
	{
		char c = file_buf[i];

		if (c == '\n')
		{
			token_push(tokens, NULL, line, TOK_NEWLINE);
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

			/* extract substr of the keyword while checking for overflow*/
			while ((is_alpha(file_buf[i]) != 0) && i < size)
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

			token_push(tokens, kw, line, check_identifier(kw));

			free(kw);
			i--;
		}
		else if (c == '"')
		{
			i++;
			/* necessary for checking the ending quote */
			int start = i;

			/* TODO: escape values such as \" \' */
			while (!(strchr("\t\n\"", file_buf[i])) && i < size)
				i++;

			if (file_buf[i] != '"')
			{
				err_push(err, "string unterminated at %lu", line);
				return;
			}

			int str_len = i - start;
			char *str = malloc(sizeof(char) * str_len + 1);
			if (!str)
			{
				fprintf(stderr, "malloc failed at lex_begin\n");
				exit(EXIT_FAILURE);
			}

			strncpy(str, &file_buf[start], str_len);
			str[str_len] = '\0';

			token_push(tokens, str, line, TOK_STRING);

			free(str);
			i++;
		}
		else if (is_digit(c))
		{
			int start = i;

			while (is_digit(file_buf[i]))
			{
				i++;
			}
			/* check if the last character after the last digit is a space
			 * so that we can get a valid number
			 */
			if (file_buf[i] != ' ')
			{
				err_push(err, "invalid number detected at %lu\n", line);
				return;
			}

			int num_len = i - start;
			char *val = malloc(sizeof(char) * num_len + 1);
			if (!val)
			{
				fprintf(stderr, "malloc failed in lex_begin\n");
				exit(EXIT_FAILURE);
			}

			strncpy(val, &file_buf[start], num_len);
			val[num_len] = '\0';

			token_push(tokens, val, line, TOK_NUMBER);

			free(val);
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
		"cx", "reg1", "reg2", "reg3", "reg0", "stdi",
		"stdo",
	};

	codes_len = sizeof(opcodes) / sizeof(opcodes[0]);
	reg_len = sizeof(registers) / sizeof(registers[0]);

	/* check the identifier against known opcodes */
	while (i < codes_len)
	{
		if (strncmp(opcodes[i], identifier, strlen(identifier)) == 0)
			return TOK_OPCODE;

		i++;
	}
	i = 0; /* reset i for registers check */

	/* check the identifier against known registers */
	while (i < reg_len)
	{
		if (strncmp(registers[i], identifier, strlen(identifier)) == 0)
			return TOK_REGISTER;

		i++;
	}

	/* identifier is not a valid value */
	return TOK_UNKNOWN;
}
