#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>

void lex_begin(char *file_buf, size_t size, Token **head);
char *lextract_text(char **buffer, size_t *cur_pos);
TokenType check_identifier(char *identifier);

#endif /* LEXER_H */
