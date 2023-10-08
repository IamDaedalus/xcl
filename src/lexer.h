#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>

void lex_begin(char *file_buf, size_t size, Token **head);

#endif /* LEXER_H */
