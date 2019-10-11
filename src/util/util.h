#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
<<<<<<< HEAD
=======
#include "../crfs/cr_API.h"
>>>>>>> develop


typedef struct
{
    int tipo;
    char* nombre;
    int bloque;

} dir;

<<<<<<< HEAD
dir* encontrar_directorio(char* path, int posicion);
dir* recorrer_path(char* path);
=======

typedef struct
{
    unsigned char* validez;
    unsigned char* nombre;
    unsigned char* puntero;
} blo;

typedef struct
{
    int puntero;
    int posicion;
    int* directos;

} ind_simple;


dir* encontrar_segundo_directorio(char* path, int posicion);
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
int agregar_primero_invalido2(int posicion, char* nombre, int puntero);
int agregar_primero_invalido(int posicion, char* nombre, int puntero);
void print_ls(char* path);
void print_all(int posicion);
int objective_kind(char* path);
char* isBin(char* path);
int agregar_carpeta_invalido2(int posicion, char* nombre, int puntero);
int agregar_carpeta_invalido(int posicion, char* nombre, int puntero);
int leer_bloques_directos( crFILE* file_desc, uint8_t* buffer, int nbytes, int maximo);
int nueva_leer(crFILE* file_desc, uint8_t* buffer, int nbytes);
int get_dir_block(char* path);
int get_entry_index(int dir_block, char* path);
int get_file_pointer(int dir_block, char* path);
void invalidate_entry(int dir_block, int entry_index);
void free_simple_indirect(int block);
void free_double_indirect(int block);
void free_triple_indirect(int block);
char* actual_locals(char* path);
char* locals_to_create(char* path);
char* get_first_folder(char* path);
void create_local_directory(char* path);
char* next_folder(char* real_path, char* new_folder);
int populate_data_block(FILE* disk_file, int block, void* buffer, int* current_buffer_pos, int length);
int populate_simple_indirect(FILE* disk_file, int block, void* buffer, int* current_buffer_pos, int length);
int populate_double_indirect(FILE* disk_file, int block, void* buffer, int* current_buffer_pos, int length);
int populate_triple_indirect(FILE* disk_file, int block, void* buffer, int* current_buffer_pos, int length);

>>>>>>> develop
