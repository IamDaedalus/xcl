#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "err.h"
#include <stddef.h>

void lex_begin(char *file_buf, size_t size, Token **tokens, Error **err);
char *lextract_text(char **buffer, size_t *cur_pos);
TokenType check_identifier(char *identifier);

#endif /* LEXER_H */
