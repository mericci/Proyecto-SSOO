#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "cr_API.h"
#include "../util/util.h"


int main(int argc, char** argv)
{
    if(argc < 2)
	{
		printf("Los parametros no fueron entregados de manera correcta");
		return 1;
	}

	cr_mount(argv[1]);
	crFILE* file = malloc(sizeof(crFILE));
	file = cr_open("/dir/subdir/altera.mp4",'r');
	int count = 0;
	printf("%d\n",file->num_bloques);
	/*for(int i = 0; i < file -> num_bloques; i++)
	{
		usleep(50000);
		printf("%d\n",file->directos[i]);
		printf("CONTADOR: %d\n", count);
		count++;
	}*/
	return 0;
}
