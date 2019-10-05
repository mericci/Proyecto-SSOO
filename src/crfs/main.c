#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "cr_API.h"


void cr_mount(char* diskname);

int main(int argc, char** argv)
{
    if(argc < 2)
	{
		printf("Los parametros no fueron entregados de manera correcta");
		return 1;
	}

	cr_mount(argv[1]);
	printf("HOLA\n");
	return 0;
}