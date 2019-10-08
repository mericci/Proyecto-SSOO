#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "cr_API.h"
#include "../util/util.h"

//ERRORES POSIBLES:
char* ERROR5 = "\n-----------------\n    Error 5:\nNo se puede crear,\nDirectorio ya existente"
"\n-----------------\n";
char* ERROR6 = "\n-----------------\n    Error 6:\nNo se puede crear,\nArchivo ya existente"
"\n-----------------\n";
char* ERROR23 =  "\n-----------------\n    Error 23:\nRuta invalida\n-----------------\n";
char* ERROR24 = "\n-----------------\n    Error 24:\nObjetivo no es \n"
"un disco virtual\n-----------------\n";
char*ERROR25 = "\n-----------------\n    Error 24:\nObjetivo no es directorio\n-----------------\n";
char* ERROR30= "\n-----------------\n    Error 30:\nFalta de Espacio\n-----------------\n";

// FUNCIONES GENERALES

void cr_mount(char* diskname)
{
    /* Funcion para montar el disco. Establece como variable global la ´
    ruta local donde se encuentra el archivo .bin correspondiente al disco */
    char* extension = isBin(diskname);
    const char* bin = ".bin";
    char* path = realpath(diskname, NULL);
    if(path == NULL){
        printf("%s\n", ERROR23);
        exit(0);
    }
    if(strcmp(extension,bin)!=0){
        printf("%s\n", ERROR24);
        exit(0);
    }
    else {
        DISK_PATH = diskname;
    };
}


void cr_bitmap(unsigned block, bool hex)
{
    /* Funcion para imprimir el ´ bitmap. Cada vez que
    se llama esta funcion, imprime en ´ stderr el estado actual del bloque de bitmap correspondiente a block
    (bitmap block ∈ {1, ..., 129}), ya sea en binario (si hex es false) o en hexadecimal (si hex es true). Si
    se ingresa block = 0, se debe imprimir el bitmap completo, imprimiendo ademas una l ´ ´ınea con la cantidad
    de bloques ocupados, y una segunda lınea con la cantidad de bloques libres. */
    FILE* disk_file = fopen(DISK_PATH, "rb");

    if (hex) {
    }
    else {
    }
    unsigned char buffer[1024];

    if (block == 0) {
        //imprimo el bitmap completo
        fseek(disk_file, 1024, SEEK_SET);
        int current_block = 0;
        int byte_number;
        int bin_array[8];
        for (int bitmap_index = 1; bitmap_index <= 128; bitmap_index++) {
            fread(buffer, 1, 1024, disk_file);
            //imprimo
            for (int i = 0; i < 1024; i++) {
                byte_number = buffer[i];
                dec_to_bin(byte_number, bin_array);
                for(int b = 7; b >= 0; b--) {
                    fprintf(stderr, "curent block: %d; state %d\n", current_block, bin_array[b]);
                    current_block += 1;
                }
            }
            //muevo el puntero al siguiente bloque de bitmap
            fseek(disk_file, 1024, SEEK_CUR);

        }
        fclose(disk_file);
        return;

    }

    //current_block is the block to be represented by the bitmap
    int current_block = 1024 * (block - 1);
    int byte_offset = block * 1024;
    fseek(disk_file, byte_offset, SEEK_SET);
    fread(buffer, 1, 1024, disk_file);
    //imprimo
    int byte_number;
    int bin_arrray[8];
    for (int i = 0; i < 1024; i++) {
        byte_number = buffer[i];
        dec_to_bin(byte_number, bin_arrray);
        for (int b = 7; b >= 0; b--) {
            fprintf(stderr, "current block: %d; state: %d\n", current_block, bin_arrray[b]);
            current_block += 1;
        }
    }
    fclose(disk_file);
}


int cr_exists(char* path)
{
    /*Funcion para ver si un archivo o carpeta existe en la ruta especificada por ´
    path. Retorna 1 si el archivo o carpeta existe y 0 en caso contrario  */
    dir* directorio = recorrer_path(path);
    if(directorio == NULL){
        return 0;
    } else {
        return 1;
    }
}


void cr_ls(char* path)
{
    /*  Funcion para listar los elementos de un directorio del disco. Imprime en pan- ´
    talla los nombres de todos los archivos y directorios contenidos en el directorio indicado por path.*/
    int is_directory = objective_kind(path);
    if (is_directory == 23){
        printf("%s\n", ERROR23);
    }
    else if(is_directory == 0){
        printf("Objetivo es archivo\n");
    }
    else if(is_directory == 1){
        print_ls(path);
    }
}


int cr_mkdir(char *foldername)
{
    /*Funcion para crear directorios. Crea el directorio vacío referido
    por foldername*/
    if(cr_exists(foldername))
    {
        printf("%s", ERROR6);
        return 0;
    }
    int block_to_create = first_free_block(); //DIFERENCIAR ENTRE INDICI Y DIRECTORIO
    char* nombre_carpeta = obtener_nombre(foldername);
    char* dir_to_append = directorio_a_agregar(foldername);
    dir* direccion = recorrer_path(dir_to_append);
    if(!direccion)
    {
        printf("NO SE PUEDE CREAR ARCHIVO POR QUE NO EXISTE DIRECTORIO\n");
        free(nombre_carpeta);
        free(dir_to_append);
        free(direccion);
        //fclose(archivo);
        return 0;
    }
    int agregar = agregar_carpeta_invalido(direccion -> bloque, nombre_carpeta, block_to_create);
    ///////////////////////////////////////////////////////if(agregar) change_bitmap_block(block_to_create); ///REVISAR FUNCION
    if(agregar)
    {
        ///AGREGAR CHANGE BITMAP BLOCK
        return 1;
    }
    return 0;

}


// FUNCIONES DE MANEJOS DE ARCHIVOS.

crFILE* cr_open(char* path, char mode)
{
    /*  Funcion para abrir un archivo. Si ´ mode es ‘r’, busca
    el archivo en la ruta path y retorna un crFILE* que lo representa. Si mode es ‘w’, se verifica que el archivo
    no exista en la ruta especificada y se retorna un nuevo crFILE* que lo representa.*/
    crFILE* nuevo_archivo = malloc(sizeof(crFILE));
    if(mode == 'r')
    {
        //printf("1\n");
        nuevo_archivo -> modo = 0;
        nuevo_archivo -> entrada = 0;
        //nuevo_archivo -> leido = 0;
        dir* direccion = malloc(sizeof(dir));
        direccion = recorrer_path(path);
        nuevo_archivo -> bloque = direccion -> bloque;
        nuevo_archivo -> bloque_actual = nuevo_archivo -> bloque;
        FILE* archivo = fopen(DISK_PATH, "r");
        //printf("hola %d\n",direccion -> tipo);
        //printf("hola %d\n",cr_exists(path));
        if(cr_exists(path) && direccion -> tipo == 4)
        {
            int bloq_indice = direccion -> bloque;
            fseek(archivo, bloq_indice*1024, SEEK_SET);
            unsigned char* tamano = malloc(4*sizeof(unsigned char));
            fread(tamano,sizeof(unsigned char),4,archivo);
            nuevo_archivo -> tamano = tamano[3] + (tamano[2] << 8) + (tamano[1] << 16) + (tamano[0] << 24);
            nuevo_archivo -> num_bloques = nuevo_archivo -> tamano/1024;
            nuevo_archivo -> directos = calloc(252,sizeof(int));
            for(int i = 0; i < 252; i++) //considerar dir indirecto;
            {
                unsigned char* bloque_ingresado = malloc(4*sizeof(unsigned char));
                fread(bloque_ingresado,sizeof(unsigned char),4,archivo);
                nuevo_archivo -> directos[i] = bloque_ingresado[3] + (bloque_ingresado[2] << 8) +
                                               (bloque_ingresado[1] << 16) + (bloque_ingresado[0] << 24);
                free(bloque_ingresado);
            }
            unsigned char* bloque_ingresado = malloc(4*sizeof(unsigned char));
            fread(bloque_ingresado,sizeof(unsigned char),4,archivo);
            nuevo_archivo -> dir1 = bloque_ingresado[3] + (bloque_ingresado[2] << 8) +
                                               (bloque_ingresado[1] << 16) + (bloque_ingresado[0] << 24);
            fread(bloque_ingresado,sizeof(unsigned char),4,archivo);
            nuevo_archivo -> dir2 = bloque_ingresado[3] + (bloque_ingresado[2] << 8) +
                                               (bloque_ingresado[1] << 16) + (bloque_ingresado[0] << 24);
            fread(bloque_ingresado,sizeof(unsigned char),4,archivo);
            nuevo_archivo -> dir3 = bloque_ingresado[3] + (bloque_ingresado[2] << 8) +
                                               (bloque_ingresado[1] << 16) + (bloque_ingresado[0] << 24);
            fread(bloque_ingresado,sizeof(unsigned char),4,archivo);
            free(bloque_ingresado);
        }
        fclose(archivo);
        free(direccion);
        return nuevo_archivo;
    }

    else if('w' == mode)
    {
        //FILE* archivo = fopen(DISK_PATH,"rb");
        nuevo_archivo -> modo = 1;
        nuevo_archivo -> entrada = 0;
        if(cr_exists(path))
        {
            printf("%s", ERROR6);
            return NULL;
        }
        //printf("\n\n");
        int block_to_create = first_free_block(); //bloque en el que estará el archivo 
        char* nombre_archivo = obtener_nombre(path); //nombre del archivo a agregar
        char* dir_to_append = directorio_a_agregar(path); //carpeta a la que se va agregar el archivo (direccion desde root)
        dir* direccion = malloc(sizeof(dir));
        direccion = recorrer_path(dir_to_append);
        if(block_to_create == -1)
        {
            printf("%s\n", ERROR30);
            free(nombre_archivo);
            free(dir_to_append);
            free(direccion);
            //fclose(archivo);
            return NULL;
        }
        if(!direccion)
        {
            printf("NO SE PUEDE CREAR ARCHIVO POR QUE NO EXISTE DIRECTORIO\n");
            free(nombre_archivo);
            free(dir_to_append);
            free(direccion);
            //fclose(archivo);
            return NULL;
        }
        //dir* prueba;
        //printf("__________________________________________\n");
        //printf("HOLAAA5\n");
        //prueba = recorrer_path(path);
        //printf("HOLAAA6\n");
        //printf("%d AUN NO AGREGO EL PATH\n", prueba);
        //printf("__________________________________________\n");
        //printf("1\n");
        //FILE* archivo = fopen(DISK_PATH,"rb");
        //printf("2\n");
        int agregar = agregar_primero_invalido(direccion -> bloque, nombre_archivo, block_to_create);
        //printf("3\n");
        ///////////////////////////////////////////////////////if(agregar) change_bitmap_block(block_to_create);
        //printf("4\n");
        //dir* prueba = malloc(sizeof(dir));
        //printf("5\n");
        //prueba = recorrer_path(dir_to_append);
        //printf("6\n");
        //printf("%s\n", prueba -> nombre);





        //fclose(archivo);
        return nuevo_archivo;
    }

    return NULL;
}


int cr_read(crFILE* file_desc, void* buffer, int nbytes)
{
    /*Funcion para leer archivos. ´
    Lee los siguientes nbytes desde el archivo descrito por file desc y los guarda en la direccion apuntada ´
    por buffer. Debe retornar la cantidad de Byte efectivamente le´ıdos desde el archivo. Esto es importante si
    nbytes es mayor a la cantidad de Byte restantes en el archivo. La lectura de read se efectua desde
    la posición del archivo inmediatamente posterior a la ultima posici ´ on le ´ ´ıda por un llamado a read */

    int byte_read = 0;  //valor de retorno, cantidad de byte efectivamente leidos.
    /*
    if(file_desc -> modo == 0) //reviso modo lectura
    {
        FILE* archivo = fopen(DISK_PATH,"rb");
        fseek(archivo, file_desc -> bloque_actual * 1024 + file_desc -> leido, SEEK_SET);

        unsigned char* bloque_ingresado = malloc(nbytes*sizeof(unsigned char));
        fread(bloque_ingresado,sizeof(unsigned char),nbytes,archivo);
        //fseek(archivo, file_desc -> posicion * 1024 + 4, SEEK_SET);


        fclose(archivo);
    }*/
    return byte_read;





}


int cr_write(crFILE* file_desc, void* buffer, int nbytes)
{
    /*Funcion para escribir archivos. Escribe en el archivo descrito por file desc los nbytes que se encuentren
    en la direccion indicada ´
    por buffer. Retorna la cantidad de Byte escritos en el archivo. Si se produjo un error porque no pudo seguir
    escribiendo, ya sea porque el disco se lleno o porque el archivo no puede crecer m ´ as, este n ´ umero puede
    ser menor a nbytes (incluso 0) */


}


int cr_close(crFILE* file_desc)
{
    /* Funcion para cerrar archivos. Cierra el archivo indicado por ´
    file desc. Debe garantizar que cuando esta funcion retorna, el archivo se encuentra actualizado en disco */
}


int cr_rm(char* path)
{
    /* Funcion para borrar archivos. Elimina el archivo referenciado por la ruta ´ path
    del directorio correspondiente. Los bloques que estaban siendo usados por el archivo deben quedar libres */
}


int cr_unload(char* orig, char* dest)
{
    /* Funcion que se encarga de copiar un archivo o un ´ arbol ´
    de directorios (es decir, un directorio y todos sus contenidos) del disco, referenciado por orig,
    hacia un nuevo
    archivo o directorio de ruta dest en su computador  */

}


int cr_load(char* orig)
{
    /*Funcion que se encarga de copiar un archivo o ´ arbol de directorios, referen- ´
    ciado por orig al disco. En caso de que un archivo sea demasiado pesado para el disco, se debe escribir todo
    lo posible hasta acabar el espacio disponible */
}
