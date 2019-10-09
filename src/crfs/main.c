#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "cr_API.h"
#include "../util/util.h"


int main(int argc, char** argv)
{
	printf("eihff");
    if(argc < 2)
	{
		printf("Los parametros no fueron entregados de manera correcta");
		return 1;
	}

	cr_mount(argv[1]);


    cr_unload("/dir/subdir", "/home/feha/Documents/Semestre12/SSOOyRR/Proyecto-SSOO/mama/porque/salsa/dds.mkv"); //#ESTE ENTRA EN LA DEMO SI O SI:
	crFILE* file = malloc(sizeof(crFILE));

	file = cr_open("/C.mkv",'r');

	int count = 0;
	//printf("%d....\n",file->num_bloques);

	uint8_t * buffer = malloc(file->tamano * sizeof(uint8_t));

	//cr_read( file, buffer, 5000);
	printf("holaaaaa");
	int lee = cr_read(file, buffer, file->tamano);

	FILE* add = fopen("progra.mkv", "wb");
	cr_close(file);
	fwrite(buffer, sizeof(uint8_t), file->tamano, add);

    //printf("%d\n",cr_exists("/dir"));
    //cr_ls("/memes");
	
	
    printf("------- %i ---------ff %i\n", file->tamano, file->leido);


	//FILE* add = fopen("laa.mkv", "wb");
	//fwrite(buffer, sizeof(uint8_t), file->tamano, add);
    //printf("%d\n",cr_exists("/dir"));
    cr_ls("/");


	//cr_ls("/dir");
    //cr_mkdir("/asdsadasd/dddsssd");
    //cr_open("/dir/subdir/altera.mp", 'r');
    //create_local_directory("/home/feha/Documents/Semestre12/SSOOyRR/Proyecto-SSOO/mama/porque");
	//cr_ls("/memes");
	//dir* direc = recorrer_path("/memes/hacker.jpg");
	//printf("%s\n", direc -> nombre);
	//int dir_block = get_dir_block("/intro.txt");
	//int entry_index = get_entry_index(dir_block, "/intro.txt");
	//printf("removed: %d\n", cr_rm("/intro.txt"));
	//invalidate_entry(dir_block, entry_index);
	//cr_ls("/");
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
