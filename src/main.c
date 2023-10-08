#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "USAGE: xcl <file>");
		exit(EXIT_FAILURE);
	}
	return 0;
}
