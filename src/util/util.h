#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef struct
{
    int tipo;
    char* nombre;
    int bloque;

} dir;

dir* encontrar_directorio(char* path, int posicion);
dir* recorrer_path(char* path);