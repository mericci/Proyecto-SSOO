#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    int modo;
    int tamano;
    int entrada;
    int num_bloques;
    int* directos;
    int dir1; //1 si se usa primer dir indirecto
    int dir2; //1 si se usa segundo dir indirecto
    int dir3; //1 si se usa tercer dir indirecto
} crFILE;

char* DISK_PATH;

void cr_mount(char* diskname);
void cr_bitmap(unsigned block, bool hex);
int cr_exists(char* path);
void cr_ls(char* path);
int cr_mkdir(char *foldername);

crFILE* cr_open(char* path, char mode);
int cr_read(crFILE* file_desc, void* buffer, int nbytes);
int cr_write(crFILE* file_desc, void* buffer, int nbytes);
int cr_close(crFILE* file_desc);
int cr_rm(char* path);
int cr_unload(char* orig, char* dest);
int cr_load(char* orig);

