#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "util.h"
#include "../crfs/cr_API.h"


dir* encontrar_directorio(char* path, int posicion) 
//recibe nombre y posicion desde donde se busca (bloque) 
//y retorna directorio de con el nombre del bloque 
{
    FILE* archivo = fopen(DISK_PATH, "rb");
    dir* directorio = malloc(sizeof(dir));
    int bloque = 0;
    fseek(archivo, posicion * 1024, SEEK_SET);
    unsigned char* validez = malloc(1*sizeof(unsigned char));
    unsigned char* nombre = malloc(27*sizeof(unsigned char));
    unsigned char* puntero = malloc(4*sizeof(unsigned char));
    for(int entrada = 0; entrada < 32; entrada++)
    {
        fread(validez,1,1,archivo);
        unsigned int* val = (unsigned int)*validez;
        if(val != 2 && val != 4 && val != 8 && val != 16 && val != 32) continue;
        fread(nombre,sizeof(unsigned char),27,archivo);
        fread(puntero,sizeof(unsigned char),4,archivo);
        bloque = puntero[3] + (puntero[2] << 8) + (puntero[1] << 16) + (puntero[0] << 24);
        //printf("%s\n", (char*)nombre);
        if(strcmp(path,(char*)nombre) == 0)
        {
            directorio -> bloque = bloque;
            directorio -> nombre = (char*)nombre;
            directorio -> tipo = (int)validez;
            return directorio;
        }
    }
    return NULL;
}

dir* recorrer_path(char* path)
{
    char* archivo = malloc(strlen(path)*sizeof(char));
    //printf("%s\n",archivo);
    int count = 0;
    int posicion = 0;
    dir* directorio = malloc(sizeof(dir));
    for(int i = 1; i < strlen(path); i++)
    {
        int letra = path[i];
        if(letra != '/')
        {
            archivo[count] = path[i];
            count++;
        }
        else
        {
            directorio = encontrar_directorio(archivo, posicion);
            if(directorio)
            {
                printf("%s         %d\n", directorio -> nombre, directorio -> bloque);
                posicion = directorio -> bloque;
                //printf("%d\n",posicion);
                count = 0;
                free(archivo);
                char* archivo = malloc(strlen(path)*sizeof(char));
                
            }
        }
        if(i == strlen(path)-1)
        { 
            //printf("%s\n",archivo);
            //printf("%d\n",posicion);
            directorio = encontrar_directorio(archivo, posicion);
            if(directorio)
            {
                printf("%s         %d\n", directorio -> nombre, directorio -> bloque);
            }
        }
    }
    return directorio;
}