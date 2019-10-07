#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../crfs/cr_API.h"


typedef struct
{
    int tipo;
    char* nombre;
    int bloque;

} dir;


typedef struct 
{
    unsigned char* validez;
    unsigned char* nombre;
    unsigned char* puntero;
} blo;


dir* encontrar_directorio(char* path, int posicion);
dir* recorrer_path(char* path);
int indirecto_simple(crFILE* archivo, int bloque_simple , int bloque_actual);
int indirecto_doble(crFILE* archivo, int bloque_doble , int bloque_actual);
int indirecto_triple(crFILE* archivo, int bloque_triple , int bloque_actual);
void dec_to_bin(int decimal, int* bin_array);
int bin_to_dec(int* bin_array);
int first_free_block();
char* obtener_nombre(char* path);
char* directorio_a_agregar(char* path);
void change_bitmap_block(int original_block);
int agregar_primero_invalido(int posicion, char* nombre, int puntero);
