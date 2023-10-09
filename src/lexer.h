#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "err.h"
#include <stddef.h>

char *lextract_str(char *file_buf, size_t *index, size_t size, size_t line);
char *lextract_num(char *file_buf, size_t *index, size_t size, size_t line);
char *lextract_id(char *file_buf, size_t *index, size_t size, size_t line);

void lex_begin(char *file_buf, size_t size, Token **tokens, Error **err);
TokenType check_identifier(char *identifier);

#endif /* LEXER_H */
