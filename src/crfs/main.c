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



	

	//LS EXISTS

	//parte1
	/*
	cr_ls("/");
	printf("\n");
	cr_ls("/dir");
	int exists = cr_exists("/dir");
	int exists2 = cr_exists("/nivi.txt");
	printf("\n");
	printf("existe /dir?: %d\n", exists);
	printf("existe /nivi?: %d\n", exists2);
	*/

	//parte2
	
	//cr_ls("/memes");


	//parte3

	//MKDIR
	/*
	cr_ls("/dir");
	printf("\n");
	cr_mkdir("/dir/new_folder");
	cr_ls("/dir");
	printf("\n");
	*/

	

	//BITMAP
	//parte 4
	//cr_bitmap(1, 0);
	
	//parte 5
	//cr_bitmap(0, 0);

	//parte 6
	//cr_bitmap(1, 1);

	//parte 7
	//cr_bitmap(0, 1);
	

	



	//CR_OPEN  CR_WRITE  CR_CLOSE CR_READ

	//parte 8
	/*
	cr_ls("/");
	printf("\n");
	crFILE* file = cr_open("/intro.txt", 'r');
	uint8_t * buffer = malloc(file->tamano * sizeof(uint8_t));
	cr_read(file, buffer, file->tamano);
	crFILE* new_file = cr_open("/dinko.txt", 'w');
	cr_write(new_file, buffer, file->tamano);
	cr_ls("/");
	cr_close(file);
	cr_close(new_file);
	free(buffer);*/

	//parte 9
	/*
	crFILE* file = cr_open("/dinko.txt", 'r');
	uint8_t * buffer = malloc(file->tamano * sizeof(uint8_t));
	cr_read(file, buffer, file->tamano);
	FILE* add = fopen("test.txt", "wb");
	fwrite(buffer, sizeof(uint8_t), file->tamano, add);
	fclose(add);
	cr_close(file);
	free(buffer);*/

	//parte 10

	/*
	cr_ls("/");
	printf("\n");
	crFILE* file = cr_open("/Withered Leaf.mp3", 'r');
	uint8_t * buffer = malloc(file->tamano * sizeof(uint8_t));
	cr_read(file, buffer, file->tamano);
	crFILE* new_file = cr_open("/music.mp3", 'w');
	cr_write(new_file, buffer, file->tamano);
	cr_ls("/");
	cr_close(file);
	cr_close(new_file);
	free(buffer);*/

	//parte 11
	/*
	crFILE* file = cr_open("/music.mp3", 'r');
	uint8_t * buffer = malloc(file->tamano * sizeof(uint8_t));
	cr_read(file, buffer, file->tamano);
	FILE* add = fopen("music.mp3", "wb");
	fwrite(buffer, sizeof(uint8_t), file->tamano, add);
	fclose(add);
	cr_close(file);
	free(buffer);*/

	
	

	
	


	//REMOVE
	/*
	cr_ls("/");
	cr_rm("/intro.txt");
	cr_ls("/");
	cr_ls("/dir");
	cr_rm("/dir/texto.txt");
	cr_ls("/dir");
	*/
	
	
	//CR_UNLOAD
	//probar write con unload, descargar musica que ya existe

	/*
	cr_unload("/dir/intro.txt", 
	"/Users/dinko/Proyecto-SSOO/mama/porque/salsa/dds.mkv"); 
	*/

	//CR_LOAD
	/*
	cr_ls("/");
	cr_load("/Users/dinko/hola.txt");
	cr_ls("/");*/
	
		
	
	return 0;
}
