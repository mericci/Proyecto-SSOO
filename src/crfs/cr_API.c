#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cr_API.h"
#include "../util/util.h"

// FUNCIONES GENERALES
char* ERROR23 =  "-----------------\n    Error 23\nEl Path no existe\n-----------------\n";
void cr_mount(char* diskname)
{
    /* Funcion para montar el disco. Establece como variable global la ´
    ruta local donde se encuentra el archivo .bin correspondiente al disco */

    //char* path = realpath(diskname, NULL);
    DISK_PATH = diskname;
}


void cr_bitmap(unsigned block, bool hex)
{
    /* Funcion para imprimir el ´ bitmap. Cada vez que
    se llama esta funcion, imprime en ´ stderr el estado actual del bloque de bitmap correspondiente a block
    (bitmap block ∈ {1, ..., 129}), ya sea en binario (si hex es false) o en hexadecimal (si hex es true). Si
    se ingresa block = 0, se debe imprimir el bitmap completo, imprimiendo ademas una l ´ ´ınea con la cantidad
    de bloques ocupados, y una segunda lınea con la cantidad de bloques libres. */



}


int cr_exists(char* path)
{
    /*Funcion para ver si un archivo o carpeta existe en la ruta especificada por ´
    path. Retorna 1 si el archivo o carpeta existe y 0 en caso contrario  */
    dir* directorio = recorrer_path(path);
    printf("Existe en la posicion %p\n", directorio);
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
    printf("Aqui empieza cr_ls \n" );
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
    /*Funcion para crear directorios. Crea el directorio vac ´ ´ıo referido
    por foldername*/
}


// FUNCIONES DE MANEJOS DE ARCHIVOS.

crFILE* cr_open(char* path, char mode)
{
    /*  Funcion para abrir un archivo. Si ´ mode es ‘r’, busca
    el archivo en la ruta path y retorna un crFILE* que lo representa. Si mode es ‘w’, se verifica que el archivo
    no exista en la ruta especificada y se retorna un nuevo crFILE* que lo representa.*/
    if('r' == mode)
    {
        if(cr_exists(DISK_PATH))
        {

        }
        else
        {

        }

    }
    else if('w' == mode)
    {

    }
    return NULL;
}


int cr_read(crFILE* file_desc, void* buffer, int nbytes)
{
    /*Funcion para leer archivos. ´
    Lee los siguientes nbytes desde el archivo descrito por file desc y los guarda en la direccion apuntada ´
    por buffer. Debe retornar la cantidad de Byte efectivamente le´ıdos desde el archivo. Esto es importante si
    nbytes es mayor a la cantidad de Byte restantes en el archivo. La lectura de read se efectua desde la posici ´ on´
    del archivo inmediatamente posterior a la ultima posici ´ on le ´ ´ıda por un llamado a read */

    int byte_read = 0;  //valor de retorno, cantidad de byte efectivamente leidos.
    FILE* archivo = fopen(DISK_PATH, "rb");

    fclose(archivo);





}


int cr_write(crFILE* file_desc, void* buffer, int nbytes)
{
    /*Funcion para escribir archivos. Escribe en el archivo descrito por file desc los nbytes que se encuentren en la direccion indicada ´
    por buffer. Retorna la cantidad de Byte escritos en el archivo. Si se produjo un error porque no pudo seguir
    escribiendo, ya sea porque el disco se lleno o porque el archivo no puede crecer m ´ as, este n ´ umero puede ser ´
    menor a nbytes (incluso 0) */

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
    de directorios (es decir, un directorio y todos sus contenidos) del disco, referenciado por orig, hacia un nuevo
    archivo o directorio de ruta dest en su computador  */

}


int cr_load(char* orig)
{
    /*Funcion que se encarga de copiar un archivo o ´ arbol de directorios, referen- ´
    ciado por orig al disco. En caso de que un archivo sea demasiado pesado para el disco, se debe escribir todo
    lo posible hasta acabar el espacio disponible */
}
