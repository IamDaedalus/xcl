#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helpers.h"
#include "lexer.h"
#include "main.h"
#include "token.h"

int main(int argc, char **argv)
{
	FILE *src_file;
	size_t buf_size = 0;
	char *file_buffer = NULL;
	Token *head = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: xcl <file>");
		exit(EXIT_FAILURE);
	}

	/* always open files with rb per the following
	 * stackoverflow.com/questions/19260209/ftell-returning-incorrect-value
	 */
	src_file = fopen(argv[1], "rb");
	if (!src_file)
	{
		fprintf(stderr, "Failed to open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}


	token_push(&head, NULL, 0, TOK_START);
	buf_size = load_file(src_file, &file_buffer);

	lex_begin(file_buffer, buf_size, &head);


	free(file_buffer);
	token_free(head);

	return 0;
}

/**
 * load_file - loads the file into memory
 * @file: a file pointer to the file on disk
 * Return: returns a buffer containing the new content
 *
 * Taken from
 * stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
 */
size_t load_file(FILE *file, char **buffer)
{
	char *buf = NULL;

	/* go the end of the file */
	fseek(file, 0, SEEK_END);
	/* get the position of the pointer in the file
	 * which is in the case the end
	 */
	long file_size = ftell(file);
	/* go back to the beginning of the file */
	rewind(file);

	buf = malloc(sizeof(char) * file_size + 1);
	if (!buf)
	{
		fprintf(stderr, "Malloc failed in load_file\n");
		exit(EXIT_FAILURE);
	}

	/* read into the buffer from the start to the end of the file */
	fread(buf, file_size, 1, file);

	buf[file_size] = '\0';

	*buffer = strdup(buf);

	free(buf);
	fclose(file);
	return file_size;
}
