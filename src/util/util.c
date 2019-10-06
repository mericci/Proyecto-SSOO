#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "util.h"



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
        if(strcmp(path,(char*)nombre) == 0)
        {
            directorio -> bloque = bloque;
            directorio -> nombre = (char*)nombre;
            directorio -> tipo = (int)validez;
            return directorio;
        }
    }
    fclose(archivo);
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
                //printf("%s         %d\n", directorio -> nombre, directorio -> bloque);
                posicion = directorio -> bloque;
                //printf("%d\n",posicion);
                count = 0;

            } else {
                free(archivo);
                char* archivo = malloc(strlen(path)*sizeof(char));
                break;
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

int indirecto_simple(crFILE* archivo, int bloque_simple , int bloque_actual)
{
    FILE* file = fopen(DISK_PATH, "rb");
    fseek(file, bloque_simple* 1024, SEEK_SET);
    int partida = bloque_actual;
    for(int j = partida; j < archivo -> num_bloques; j++)
    {
        unsigned char* bloque_ingresado = malloc(4*sizeof(unsigned char));
        fread(bloque_ingresado,sizeof(unsigned char),4,archivo); 
        archivo -> directos[j] = bloque_ingresado[3] + (bloque_ingresado[2] << 8) +
                                 (bloque_ingresado[1] << 16) + (bloque_ingresado[0] << 24);
        free(bloque_ingresado);
        bloque_actual++;
        if(bloque_actual - partida >= 256) break;
    }
    fclose(file);
    return bloque_actual;
}

int indirecto_doble(crFILE* archivo, int bloque_doble , int bloque_actual)
{
    FILE* file = fopen(DISK_PATH, "rb");
    fseek(file, bloque_doble* 1024, SEEK_SET);
    int num_simple = 0;
    while(bloque_actual < archivo->num_bloques && num_simple < 256){
        unsigned char* bloq_simple_raw = malloc(4*sizeof(unsigned char));
        int bloq_simple; 
        fread(bloq_simple_raw,sizeof(unsigned char),4,file);
        bloq_simple = bloq_simple_raw[3] + (bloq_simple_raw[2] << 8) + 
                        (bloq_simple_raw[1] << 16) + (bloq_simple_raw[0] << 24);
        
        fseek(file, bloq_simple* 1024, SEEK_SET);
        bloque_actual = indirecto_simple(archivo, bloq_simple, bloque_actual);

        num_simple += 1;
    }
    fclose(file);
    return bloque_actual;
}


int indirecto_triple(crFILE* archivo, int bloque_triple , int bloque_actual)
{
    FILE* file = fopen(DISK_PATH, "rb");
    fseek(file, bloque_triple* 1024, SEEK_SET);
    int num_doble = 0;
    while(bloque_actual < archivo->num_bloques && num_doble < 256){
        unsigned char* bloq_doble_raw = malloc(4*sizeof(unsigned char));
        int bloq_doble; 
        fread(bloq_doble_raw,sizeof(unsigned char),4,file);
        bloq_doble = bloq_doble_raw[3] + (bloq_doble_raw[2] << 8) + 
                        (bloq_doble_raw[1] << 16) + (bloq_doble_raw[0] << 24);
        
        fseek(file, bloq_doble* 1024, SEEK_SET);
        bloque_actual = indirecto_doble(archivo, bloq_doble, bloque_actual);

        num_doble += 1;
    }
    fclose(file);
    return bloque_actual;
}
