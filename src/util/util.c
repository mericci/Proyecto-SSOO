#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
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
        if(val != 2 && val != 4 && val != 8 && val != 16 && val != 32)
        {
            fseek(archivo, 31, SEEK_CUR);
            continue;
        }
        fread(nombre,sizeof(unsigned char),27,archivo);
        fread(puntero,sizeof(unsigned char),4,archivo);
        bloque = puntero[3] + (puntero[2] << 8) + (puntero[1] << 16) + (puntero[0] << 24);
        if(strcmp(path,(char*)nombre) == 0)
        {
            directorio -> bloque = bloque;
            directorio -> nombre = (char*)nombre;
            //int val = (int)validez[3];
            directorio -> tipo = val;
            fclose(archivo);
            return directorio;
        }
    }
    fclose(archivo);
    return NULL;
}

dir* recorrer_path(char* path)
{
    char* archivo = calloc(strlen(path),sizeof(char));
    //printf("%s\n", archivo);
    //printf("%s\n", path);
    int count = 0;
    int posicion = 0;
    dir* directorio = malloc(sizeof(dir));
    for(int i = 1; i < strlen(path); i++){
        int letra = path[i];
        if(letra != '/'){
            archivo[count] = path[i];
            //printf("ARME ES ARCHIVO: %s\n", archivo);
            count++;
        }
        else{
            directorio = encontrar_directorio(archivo, posicion);
            if(directorio)
            {
                //printf("\n%s\n", path);
                //printf("%s\n\n", archivo);
                posicion = directorio -> bloque;
                count = 0;
            } else {
                printf("\n error con: %s\n\n", archivo);

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

int bin_to_dec(int* bin_array) {
    int dec = 0;
    for (int i = 0; i < 8; i++) {
        dec += bin_array[i] * pow(2, i);
    }
    return dec;
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
    char* archivo = calloc(strlen(path),sizeof(char));
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
    int len = 0;
    int pivote = 0;
    for(int i = 0; i < strlen(path); i++)
    {
        if(path[i] == '/')
        {
            len+=pivote;
            pivote = 0;
        }
        pivote++;
    }
    char* directorio = calloc(len,sizeof(char));
    for(int j = 0; j < len; j++)
    {
        directorio[j] = path[j];
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

void print_all(int posicion){
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

void change_bitmap_block(int original_block) {
    unsigned char buffer[1] = {1};
    int bin_array[8];
    int bitmap_block = 1 + (original_block / 8192);
    int byte_offset = original_block % 8192 / 8;
    int bit_to_modify = original_block % 8;
    FILE* disk_file = fopen(DISK_PATH, "rb");
    fseek(disk_file, bitmap_block * 1024 + byte_offset, SEEK_SET);
    fread(buffer, 1, 1, disk_file);

    int byte_dec = buffer[0];
    dec_to_bin(byte_dec, bin_array);

    if (bin_array[7 -bit_to_modify] == 1) {
        bin_array[7 - bit_to_modify] = 0;
    } else {
        bin_array[7 - bit_to_modify] = 1;

    }

    fclose(disk_file);
    int arch = bin_to_dec(bin_array);
    unsigned char *new_byte_value = malloc(1*sizeof(unsigned char));
    *new_byte_value = (unsigned char)arch;
    disk_file = fopen(DISK_PATH, "rb+");
    fseek(disk_file, bitmap_block * 1024 + byte_offset, SEEK_SET);
    fwrite(new_byte_value, 1, 1, disk_file);
    fclose(disk_file);

}

void print_ls(char* path){
    char* archivo = calloc(strlen(path), sizeof(char));
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

int agregar_primero_invalido(int posicion, char* nombre, int puntero) //falta agregar carpeta bloque del directorio??
//recibe nombre y posicion desde donde se busca (bloque)
//y retorna directorio de con el nombre del bloque
{
    FILE* archivo = fopen(DISK_PATH, "rb");
    //blo* bloque = malloc(sizeof(blo));
    fseek(archivo, posicion *1024, SEEK_SET);
    unsigned char* validez = malloc(1*sizeof(unsigned char));
    unsigned char* name = malloc(27*sizeof(unsigned char));
    unsigned char* punt = malloc(4*sizeof(unsigned char));
    //unsigned char* salto = malloc(31*sizeof(unsigned char));
    for(int entrada = 0; entrada < 32; entrada++)
    {
        fread(validez,1,1,archivo);
        unsigned int* val = (unsigned int)*validez;
        if(val != 2 && val != 4 && val != 8 && val != 16 && val != 32)
        {
            fclose(archivo);
            int arch = 4;
            archivo = fopen(DISK_PATH, "rb+");
            fseek(archivo, posicion * 1024 + entrada * 32, SEEK_SET);
            *validez = (unsigned char)arch;
            int p = puntero;
            for(int j=3; j >= 0; j--)
            {
                punt[j] = (unsigned char) (p % 256);
                p = p / 256;
            }
            strcpy((char*)name,nombre);
            fwrite(validez,1,1,archivo);
            fwrite(nombre,1,27,archivo);
            fwrite(punt,1,4,archivo);
            fclose(archivo);
            /*
            FILE* hola = fopen(DISK_PATH, "rb");
            fseek(hola, posicion * 1024 + entrada * 32, SEEK_SET);
            fread(validez,1,1,hola);
            printf("LA VALIDEZ ES: %d\n", (unsigned int)*validez);
            fread(name,1,27,hola);
            printf("EL NOMBRE ES: %d\n", (char)*name);
            fread(punt,1,4,hola);
            int bloque;
            bloque = punt[3] + (punt[2] << 8) + (punt[1] << 16) + (punt[0] << 24);
            printf("el bloque es: %d\n", puntero);
            printf("LA POSICION ES: %d\n", bloque);
            fclose(hola);
            */
            return 1;
        }
        fseek(archivo, 31, SEEK_CUR);

    }
    fclose(archivo);
    return 0;
}
char* isBin(char* path){
    char* extension = calloc(4,sizeof(char));
    int len = strlen(path) - 1;
    int count = 3;
    for(int i = len; i > 0; i--){
        extension[count] = path[i];
        count = count - 1;
    }
    return extension;
}


int agregar_carpeta_invalido(int posicion, char* nombre, int puntero) //falta agregar carpeta bloque del directorio??
//recibe nombre y posicion desde donde se busca (bloque)
//y retorna directorio de con el nombre del bloque
{
    FILE* archivo = fopen(DISK_PATH, "rb");
    //blo* bloque = malloc(sizeof(blo));
    fseek(archivo, posicion *1024, SEEK_SET);
    unsigned char* validez = malloc(1*sizeof(unsigned char));
    unsigned char* name = malloc(27*sizeof(unsigned char));
    unsigned char* punt = malloc(4*sizeof(unsigned char));
    //unsigned char* salto = malloc(31*sizeof(unsigned char));
    for(int entrada = 0; entrada < 32; entrada++)
    {
        fread(validez,1,1,archivo);
        unsigned int* val = (unsigned int)*validez;

        if(val != 2 && val != 4 && val != 8 && val != 16 && val != 32)
        {
            fclose(archivo);
            int arch = 2;
            archivo = fopen(DISK_PATH, "rb+");
            fseek(archivo, posicion * 1024 + entrada * 32, SEEK_SET);
            *validez = (unsigned char)arch;
            int p = puntero;
            for(int j=3; j >= 0; j--)
            {
                punt[j] = (unsigned char) (p % 256);
                p = p / 256;
            }
            strcpy((char*)name,nombre);
            fwrite(validez,1,1,archivo);
            fwrite(nombre,1,27,archivo);
            fwrite(punt,1,4,archivo);
            fclose(archivo);
            archivo = fopen(DISK_PATH, "rb+");
            fseek(archivo, puntero * 1024 + 32, SEEK_SET);
            char* punto = ".";
            strcpy((char*)name,punto);
            arch = 8;
            *validez = (unsigned char)arch;
            fwrite(validez,1,1,archivo);
            fwrite(name,1,27,archivo);
            fwrite(punt,1,4,archivo);
            fclose(archivo);
            archivo = fopen(DISK_PATH, "rb+");
            fseek(archivo, puntero * 1024, SEEK_SET);
            int padre = posicion;
            punto = "..";
            strcpy((char*)name,punto);
            for(int j=3; j >= 0; j--)
            {
                punt[j] = (unsigned char) (padre % 256);
                padre = padre / 256;
            }
            arch = 16;
            *validez = (unsigned char)arch;
            fwrite(validez,1,1,archivo);
            fwrite(name,1,27,archivo);
            fwrite(punt,1,4,archivo);
            fclose(archivo);
            /*
            FILE* hola = fopen(DISK_PATH, "rb");
            fseek(hola, posicion * 1024 + entrada * 32, SEEK_SET);
            fread(validez,1,1,hola);
            printf("LA VALIDEZ ES: %d\n", (unsigned int)*validez);
            fread(name,1,27,hola);
            printf("EL NOMBRE ES: %d\n", (char)*name);
            fread(punt,1,4,hola);
            int bloque;
            bloque = punt[3] + (punt[2] << 8) + (punt[1] << 16) + (punt[0] << 24);
            printf("el bloque es: %d\n", puntero);
            printf("LA POSICION ES: %d\n", bloque);
            fclose(hola);
            */
            return 1;
        }
        fseek(archivo, 31, SEEK_CUR);
    }
    fclose(archivo);
    return 0;
}

int leer_bloques_directos( crFILE* file_desc, uint8_t* buffer, int nbytes)
{
    FILE* file = fopen(DISK_PATH, "rb");
    int leido;
    leido=0;
    printf("%d\n", file_desc->bloque_actual);
    uint8_t lectura = 0;
    uint8_t dato=0;

    while (file_desc-> bloque_actual < 252 && leido < nbytes)
    {

        printf("itero");
        //leer bloque directo
        //posicionamos en bloque actual y linea actual de ese bloque
        fseek(file, file_desc->directos[file_desc-> bloque_actual] * 1024 + file_desc->posicion_en_bloque , SEEK_SET);

        //caso podemos terminar con la lectura del restante
        if ( (1024 - file_desc->posicion_en_bloque) > nbytes  )
        {

            printf("Posicion de bloque actual es el: %i\n", file_desc->bloque_actual);

            printf("Posicion dentro de bloque: %i\n", file_desc->posicion_en_bloque);

            for (int i = 0; i < nbytes; i++)
            {
                fread(lectura,sizeof(uint8_t),1,file);
                buffer[dato] = lectura;
                dato+=1;

            }
            fread(lectura,sizeof(uint8_t),1,file);
                buffer[dato] = lectura;

            leido += nbytes;
            file_desc ->posicion_en_bloque += nbytes;






        }
        else if ((1024 - file_desc->posicion_en_bloque) == nbytes)
        {

            printf("Posicion de bloque actual es el: %i\n", file_desc->bloque_actual);
            printf("Posicion dentro de bloque: %i\n", file_desc->posicion_en_bloque);
            for (int i = 0; i < nbytes; i++)
            {
                fread(lectura,sizeof(uint8_t),1,file);
                buffer[dato] = lectura;
                dato+=1;
            }
            leido += nbytes;
            file_desc ->posicion_en_bloque = 0;
            file_desc ->bloque_actual += 1;



        }


        else
        {

            printf("Posicion de bloque actual es el: %i\n", file_desc->bloque_actual);
            printf("Posicion dentro de bloque: %i\n", file_desc->posicion_en_bloque);
            printf("lei %i bytes", leido);
            int maximo_posible = (1024 - file_desc->posicion_en_bloque);

            for (int i = 0; i < maximo_posible; i++)
            {

                fread(lectura,sizeof(uint8_t),1,file);
                buffer[dato] = lectura;
                dato+=1;
            }
            leido += maximo_posible;
            file_desc ->posicion_en_bloque = 0;
            file_desc ->bloque_actual += 1;


        }


    }

    printf("-------%i-------\n", file_desc-> bloque_actual);


    fclose(file);
}

char* actual_locals(char* path){
    /*Retorna el path de directorios que ya existen en el local.
    Retorna NULL si no hay ningun directorio real.*/
    bool checked = false;
    char* actual;
    DIR* direccion = opendir(path);
    if(direccion){
        return path;
    }
    actual = directorio_a_agregar(path);
    while(!checked){
        direccion = opendir(actual);
        if(direccion){
            return actual;
        } if(strcmp(actual,"")==0){
            checked = true;
            printf("estamos en nulo\n" );
            return NULL;
        }
        actual = directorio_a_agregar(actual);
    }
}

char* locals_to_create(char* path){
    /* Retorna la parte del path que no existe en local*/
    char* existent = actual_locals(path);
    int path_len = strlen(path);
    int exist_len = strlen(existent);
    char* aux = calloc(path_len - exist_len, sizeof(char));
    int count = 0;
    for(int i=0; i<path_len; i++){
        if(i>exist_len - 1){
            aux[count] = path[i];
            count++;
        }
    }
    return aux;
}

char* get_first_folder(char* path){
    int count = 1;
    int len = strlen(path);
    for(int i=1; i < len; i++){
        if(path[i]!='/'){
            count++;
        }
        else break;
    }
    char* aux = calloc(count,sizeof(char));
    for(int i=0; i<count;i++){
        aux[i]=path[i];
    }
    return aux;
}

char* next_folder(char* real_path, char* new_folder){
    int path_len = strlen(real_path);
    int folder_len = strlen(new_folder);
    printf("%s\n",new_folder );
    int total = path_len + folder_len;
    int count = 0;
    char* aux = calloc(total, sizeof(char));
    for(int i = 0; i < path_len; i++){
        aux[i] = real_path[i];
        count++;
    }
    for(int i = 0; i < folder_len; i++){
        aux[count] = new_folder[count];
        count++;
    }
    printf("%s\n",aux );
    return aux;
}

void create_local_directory(char* path){
    /* Recibe el path absoluto en el local donde se quiere crear*/
    char* existent = actual_locals(path);
    if(existent){
        if(strcmp(existent,path)==0){
            printf("El path completo existe\n%s\n%s\n", existent, path);
        }else{
            char* new = locals_to_create(path);
            char* first = get_first_folder(new);
            char* new_folder_path = next_folder(existent, first);
            int status;
            status = mkdir(new_folder_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }
}
