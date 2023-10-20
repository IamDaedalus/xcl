#include "lexer.h"
#include "err.h"
#include "helpers.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>

/**
 * lex_begin - this the lexer's in function. scans the source file byte
 * by byte while looking out for specific tokens such as quotes, numbers
 * and letters
 * @file_buf: the file buffer that we loaded into memory
 * @size: the size of the source file in memory. this is needed to make
 * sure the lexer doesn't read beyond the source file's memory space
 * @tokens: the doubly linked list that stores each token extracted
 * @err: linked list to hold every error we encounter. currently not used
 */
void lex_begin(char *file_buf, size_t size, Token **tokens, Error **err)
{
	(void)err;
	size_t line = 1;
	size_t index = 0;

	/* make sure the index never reads beyond the file's size */
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
			/* string extraction
			 * TODO; handle non-terminated string
			 */
			index++;
			char *str = lextract_str(file_buf, &index, size, line);
			token_push(tokens, str, line, TOK_STRING);

			free(str);
			index++;
		}
		else if (c == '-' && is_digit(file_buf[index + 1]))
		{
			/* negative number found */
		}
		else if (is_digit(c))
		{
			/* digit extraction.
			 * TODO; extract negative numbers and floats
			 */
			char *val = lextract_num(file_buf, &index, size, line);
			token_push(tokens, val, line, TOK_NUMBER);

			free(val);
		}
		index++;
	}
}

/* BIG TODO; MOST OF THE LEXTRACT FUNCTIONS DO SIMILAR THINGS
 * FIND A WAY TO MERGE THEM BY REUSING PARTS
 */

/**
 * lextract_id - this extracts an identifier when it encounters a letter
 * @file_buf: the buffer stored in memory
 * @index: the current position of the scanner
 * @size: the size of the file buffer
 * @line: the current line; incremented whenever we hit a new line
 */
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

/**
 * lextract_str - extracts a string from in between a pair of quotes
 * @file_buf: the buffer stored in memory
 * @index: the current position of the scanner
 * @size: the size of the file buffer
 * @line: the current line; incremented whenever we hit a new line
 *
 * Return: returns a string containing the string...
 */
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

/**
 * lextract_num - extracts integer numbers; doesn't handle
 * negatives or floats
 * @file_buf: the buffer stored in memory
 * @index: the current position of the scanner
 * @size: the size of the file buffer
 * @line: the current line; incremented whenever we hit a new line
 *
 * Return: returns a string containing the number
 */
char *lextract_num(char *file_buf, size_t *index, size_t size, size_t line, int negative)
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
		/* insert an error into the linked list here */
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

/**
 * check_identifier - this checks an extracted identifier against
 * arrays of known identifiers. if the identifier is not found
 * we return an unknown token
 * @identifier: the identifier we are looking at
 * 
 * Return: returns the TokenType be it UNKNOWN or something else
 */
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
