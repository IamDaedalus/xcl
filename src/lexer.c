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
	(void)err;
	size_t line = 1;
	size_t index = 0;

	while (index < size)
	{
		char c = file_buf[index];

		if (c == '\n')
		{
			token_push(tokens, NULL, line, TOK_NEWLINE);
			line++;
		}
		else if (c == '#')
		{
			while (file_buf[index] != '\n')
				index++;
		}
		else if (is_alpha(c) != 0)
		{
			char *kw = lextract_id(file_buf, &index, size, line);
			token_push(tokens, kw, line, check_identifier(kw));

			free(kw);
			index--;
		}
		else if (c == '"')
		{
			index++;
			char *str = lextract_str(file_buf, &index, size, line);
			token_push(tokens, str, line, TOK_STRING);

			free(str);
			index++;
		}
		else if (is_digit(c))
		{
			char *val = lextract_num(file_buf, &index, size, line);
			token_push(tokens, val, line, TOK_NUMBER);

			free(val);
		}
		index++;
	}
}

char *lextract_id(char *file_buf, size_t *index, size_t size, size_t line)
{
	(void)line;
	int start = *index;

	/* extract substr of the keyword while checking for overflow*/
	while ((is_alpha(file_buf[*index]) != 0) && *index < size)
		(*index)++;

	int kw_len = *index - start;
	char *kw = malloc(sizeof(char) * kw_len + 1);
	if (!kw)
	{
		fprintf(stderr, "malloc failed at lex_begin\n");
		exit(EXIT_FAILURE);
	}

	strncpy(kw, &file_buf[start], kw_len);
	kw[kw_len] = '\0';

	return kw;
}

char *lextract_str(char *file_buf, size_t *index, size_t size, size_t line)
{
	/* necessary for checking the ending quote */
	int start = *index;

	/* TODO: escape values such as \" \' */
	while (!(strchr("\t\n\"", file_buf[*index])) && *index < size)
		(*index)++;

	if (file_buf[*index] != '"')
	{
		fprintf(stderr, "no matching \" found for string on line %lu\n", line);
		exit(EXIT_FAILURE);
	}

	int str_len = *index - start;
	char *str = malloc(sizeof(char) * str_len + 1);
	if (!str)
	{
		fprintf(stderr, "malloc failed at lex_begin\n");
		exit(EXIT_FAILURE);
	}

	strncpy(str, &file_buf[start], str_len);
	str[str_len] = '\0';

	return str;
}

char *lextract_num(char *file_buf, size_t *index, size_t size, size_t line)
{
	int start = *index;

	while (is_digit(file_buf[*index]) && *index < size)
	{
		(*index)++;
	}
	/* check if the last character after the last digit is a space
	 * so that we can get a valid number
	 */
	if (file_buf[*index] != ' ')
	{
		fprintf(stderr, "invalid number at line %lu\n", line);
		printf("%s\n", file_buf);
		exit(EXIT_FAILURE);
	}

	int num_len = *index - start;
	char *val = malloc(sizeof(char) * num_len + 1);
	if (!val)
	{
		fprintf(stderr, "malloc failed in lex_begin\n");
		exit(EXIT_FAILURE);
	}

	strncpy(val, &file_buf[start], num_len);
	val[num_len] = '\0';

	return val;
}

TokenType check_identifier(char *identifier)
{
	int codes_len = 0, reg_len = 0;
	int index = 0;

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
	while (index < codes_len)
	{
		if (strncmp(opcodes[index], identifier, strlen(identifier)) == 0)
			return TOK_OPCODE;

		index++;
	}
	index = 0; /* reset index for registers check */

	/* check the identifier against known registers */
	while (index < reg_len)
	{
		if (strncmp(registers[index], identifier, strlen(identifier)) == 0)
			return TOK_REGISTER;

		index++;
	}

	/* identifier is not a valid value */
	return TOK_UNKNOWN;
}
