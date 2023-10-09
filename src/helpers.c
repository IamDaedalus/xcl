#include "helpers.h"

char is_alpha(char c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return c;
	else return 0;
}

int is_digit(char c)
{
	return c >= 48 && c <= 57;
}
