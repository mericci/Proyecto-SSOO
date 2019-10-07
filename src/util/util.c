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
    for(int entrada = 0; entrada < 32; entrada++){
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
            //int val = (int)validez[3];
            directorio -> tipo = val;
            return directorio;
        }
    }
    fclose(archivo);
    return NULL;
}

dir* recorrer_path(char* path)
{
    char* archivo = calloc(strlen(path), sizeof(char));
    int count = 0;
    int posicion = 0;
    dir* directorio = malloc(sizeof(dir));
    for(int i = 1; i < strlen(path); i++){
        int letra = path[i];
        if(letra != '/'){
            archivo[count] = path[i];
            count++;
        }
        else{
            directorio = encontrar_directorio(archivo, posicion);
            if(directorio)
            {
                //printf("%s         %d\n", directorio -> nombre, directorio -> bloque);
                posicion = directorio -> bloque;
                count = 0;
            } else {
                free(archivo);
                char* archivo = malloc(strlen(path)*sizeof(char));
                break;
            }

        }
        if(i == strlen(path)-1){
            directorio = encontrar_directorio(archivo, posicion);
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
        fread(bloque_ingresado,sizeof(unsigned char),4,file);
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
    }
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

void dec_to_bin(int decimal, int* bin_array){
    int current_result = decimal;
    for (int i = 0; i < 8; i++) {

        if (current_result == 1) {
            bin_array[i] = 1;
        } else if (current_result == 0) {
            bin_array[i] = 0;
        } else {
            bin_array[i] = current_result % 2;
            current_result = current_result / 2;

        }
    }
}

//retorna el numero del primer bloque que esta libre
int first_free_block() {
    FILE* disk_file = fopen(DISK_PATH, "rb");
    unsigned char buffer[1024];
    int current_block = 0;
    int byte_number;
    int bin_array[8];
    fseek(disk_file, 1024, SEEK_SET);
    for (int bitmap_index = 1; bitmap_index <= 128; bitmap_index++) {
        fread(buffer, 1, 1024, disk_file);
        for (int i = 0; i < 1024; i++) {
            byte_number = buffer[i];
            dec_to_bin(byte_number, bin_array);
            for (int b = 7; b >= 0; b--) {
                if (bin_array[b] == 0) {
                    fclose(disk_file);
                    return current_block;
                }
                current_block ++;
            }
        }
        fseek(disk_file, 1024, SEEK_CUR);
    }

    fclose(disk_file);

    return -1;

}


char* obtener_nombre(char* path)
{
    char* archivo = malloc(strlen(path)*sizeof(char));
    int count = 0;
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
            count = 0;
            free(archivo);
            char* archivo = malloc(strlen(path)*sizeof(char));
        }


    }
    return archivo;
}

char* directorio_a_agregar(char* path)
{
    char* directorio = malloc(strlen(path)*sizeof(char));
    int count = 0;
    for(int i = 1; i < strlen(path); i++)
    {
        int letra = path[i];
        if(letra == '/') count++;
    }
    int comp = 0;
    for(int i = 1; i < strlen(path); i++)
    {
        int letra = path[i];
        if(letra == '/') comp++;
        if(comp == count) break;
        directorio[i] = path[i];
    }
    return directorio;
}
int objective_kind(char* path){
    char h = path;
    if(strlen(path)==1){
        return 1;
    }
    dir* directorio = recorrer_path(path);
    if (directorio == NULL){
        return 23;
    }
    if (directorio->tipo == 2){
        return 1;
    }
    return 0;
}

void print_all(int posicion)
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
    for(int entrada = 0; entrada < 32; entrada++){
        fread(validez,1,1,archivo);
        unsigned int* val = (unsigned int)*validez;
        if(val != 2 && val != 4 && val != 8 && val != 16 && val != 32) continue;
        fread(nombre,sizeof(unsigned char),27,archivo);
        fread(puntero,sizeof(unsigned char),4,archivo);
        bloque = puntero[3] + (puntero[2] << 8) + (puntero[1] << 16) + (puntero[0] << 24);
        printf("%s\n",nombre );
    }
    fclose(archivo);
}


void print_ls(char* path){
    char* archivo = malloc(strlen(path)*sizeof(char));
    int count = 0;
    int posicion = 0;
    dir* directorio = malloc(sizeof(dir));
    if(strlen(path)==1){
        print_all(0);
        return;
    }
    for(int i = 1; i < strlen(path); i++){
        int letra = path[i];
        if(letra != '/'){
            archivo[count] = path[i];
            count++;
        }
        else{
            directorio = encontrar_directorio(archivo, posicion);
            if(directorio){
                posicion = directorio -> bloque;
                count = 0;

            } else {
                free(archivo);
                char* archivo = malloc(strlen(path)*sizeof(char));
                break;
            }
        }
        if(i == strlen(path)-1){
            directorio = encontrar_directorio(archivo, posicion);
            if(directorio){
                print_all(directorio -> bloque);
            }
        }
    }
}

char* isBin(char* path){
    char* extension = calloc(4,sizeof(char));
    int len = strlen(path) - 1;
    int count = 3;
    for(int i = len; i > 0; i--){
        extension[count] = path[i];
        count = count - 1;
    }
    printf("%s\n", extension);
    return extension;
}
