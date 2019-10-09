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
    //printf("%d\n",cr_exists("/dir"));
    //cr_ls("/memes");

	
	
	//cr_mkdir("/dir/martin");
	//cr_ls("/");
	
	int dir_block = get_dir_block("/dinko.txt");
	int entry_index = get_entry_index(dir_block, "/dinko.txt");
	//printf("first free block: %d\n", first_free_block());

	crFILE* file = cr_open("/dinko.txt", 'r');
	void* buffer;
	cr_write(file, buffer, 65);
	free(file);	
	cr_ls("/");
	//dir* direct = recorrer_path("/intro.txt");
	//printf("%d\n", direct->bloque);
	
	//crFILE* file = malloc(sizeof(crFILE));
	//file = cr_open("/intro.txt",'r');
	//int count = 0;
	//crFILE* file;
	//file = cr_open("/intro.txt",'r');
	//int count = 0;
	//dir* direct = encontrar_directorio("dir",0);
	//printf("%s\n",direct -> nombre);
	//dir* direct = encontrar_directorio("dir",0);
	//printf("%s\n",direct -> nombre);
	//file = cr_open("/dir/subdir/dan.txt",'w');
	//dir* direct = encontrar_directorio("memes",0);
	//printf("%s\n",direct -> nombre);
	//direct = encontrar_directorio("dir",0);
	//printf("%s\n",direct -> nombre);
	//recorrer_path("/memes");
	//printf("%d\n",file->num_bloques);
	/*for(int i = 0; i < file -> num_bloques; i++)
	{
		usleep(50000);
		printf("%d\n",file->directos[i]);
		printf("CONTADOR: %d\n", count);
		count++;
	/*
	recorrer_path("/memes/sudo1.jpg");
    printf("%d\n", cr_exists("/dir/subdir/alter.mp4"));

	crFILE* file = malloc(sizeof(crFILE));
	file = cr_open("/memes/hello.jpg",'r');
	for(int i = 0; i < file -> num_bloques; i++)
	{
		printf("%d\n",file->directos[i]);
	}*/
	return 0;
}
