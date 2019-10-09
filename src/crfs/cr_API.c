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
char* ERROR22= "\n-----------------\n    Error 22:\nNo existe directorio\n\   especificado\n-----------------\n";
char* ERROR23 =  "\n-----------------\n    Error 23:\n  Ruta invalida\n-----------------\n";
char* ERROR24 = "\n-----------------\n    Error 24:\nObjetivo no es \n"
"un disco virtual\n-----------------\n";
char*ERROR25 = "\n-----------------\n    Error 25:\nObjetivo no es directorio\n-----------------\n";
char* ERROR30= "\n-----------------\n    Error 30:\nFalta de Espacio\n-----------------\n";
char* FUT1="\n-----------------\nFeature Futura:\nEstamos trabajando\n  para poder \n  hacer esto ;)\n-----------------\n";

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
        free(directorio);
        return 0;
    } else {
        free(directorio);
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
        printf("%s\n", ERROR25);
    }
    else if(is_directory == 4){
        printf("%s\n", ERROR25);
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
        printf("%s\n", ERROR22);
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
        change_bitmap_block(block_to_create);
        int block_extension = first_free_block();
        change_bitmap_block(block_extension);
        FILE* archivo = fopen(DISK_PATH, "rb+");
        fseek(archivo, block_to_create * 1024 + 31*32, SEEK_SET);
        unsigned char* validez = malloc(1*sizeof(unsigned char));
        unsigned char* name = malloc(27*sizeof(unsigned char));
        unsigned char* punt = malloc(4*sizeof(unsigned char));
        int val = 32;
        *validez = (unsigned char)val;
        int p = block_to_create;
        for(int j=3; j >= 0; j--)
        {
            punt[j] = (unsigned char) (p % 256);
            p = p / 256;
        }
        char* nombre = calloc(2,sizeof(char));
        nombre = "";
        strcpy((char*)name,nombre);
        fwrite(validez,1,1,archivo);
        fwrite(nombre,1,27,archivo);
        fwrite(punt,1,4,archivo);
        fclose(archivo);

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
        if(cr_exists(path)==0){
            printf("%s\n", ERROR23);
            free(nuevo_archivo);
            return NULL;
        }
        nuevo_archivo -> modo = 0;
        nuevo_archivo -> entrada = 0;
        nuevo_archivo -> leido = 0;
        dir* direccion = malloc(sizeof(dir));
        direccion = recorrer_path(path);
        nuevo_archivo -> bloque = direccion -> bloque;
        nuevo_archivo -> bloque_actual = 0;
        FILE* archivo = fopen(DISK_PATH, "r");
        //printf("hola %d\n",direccion -> tipo);
        if(cr_exists(path)==1 && direccion -> tipo == 4)
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
            printf("%s\n", ERROR22);
            free(nombre_archivo);
            free(dir_to_append);
            free(direccion);
            //fclose(archivo);
            return NULL;
        }
        int agregar = agregar_primero_invalido(direccion -> bloque, nombre_archivo, block_to_create);
        if(agregar) change_bitmap_block(block_to_create);

        int bloque_a_reservar;

        FILE* archivo = fopen(DISK_PATH, "rb+");
        unsigned char* punt = malloc(4*sizeof(unsigned char));
        for(int k = 1; k < 256; k++)
        {
            bloque_a_reservar = first_free_block();
            if(bloque_a_reservar != -1)
            {
                fseek(archivo, block_to_create * 1024 + k * 4, SEEK_SET);
                change_bitmap_block(bloque_a_reservar);
                int p = bloque_a_reservar;
                for(int j=3; j >= 0; j--)
                {
                    punt[j] = (unsigned char) (p % 256);
                    p = p / 256;
                }
                fwrite(punt,1,4,archivo);

            }
        }
        free(punt);
        fclose(archivo);

        nuevo_archivo -> bloque = block_to_create;
        nuevo_archivo -> bloque_actual = block_to_create;
        nuevo_archivo -> modo = 1;
        nuevo_archivo -> tamano = 0;
        nuevo_archivo -> posicion_en_bloque = 1; //parto en segunda linea, c/linea tiene 4 bytes.
        nuevo_archivo -> dir1 = 0;
        nuevo_archivo -> dir2 = 0;
        nuevo_archivo -> dir3 = 0;
        nuevo_archivo -> uso_dir1 = 0;
        nuevo_archivo -> uso_dir2 = 0;
        nuevo_archivo -> uso_dir3 = 0;
        nuevo_archivo -> posicion_en_dir1 = 0;
        nuevo_archivo -> posicion_en_dir2 = 0;
        nuevo_archivo -> posicion_en_dir3 = 0;
        nuevo_archivo -> leido = 0;
        return nuevo_archivo;
    }

    return NULL;
}


int cr_read(crFILE* file_desc, void* buffer, int nbytes)
{
    int leido = nueva_leer(file_desc,buffer, nbytes);
    return leido;

    /*Funcion para leer archivos. ´
    Lee los siguientes nbytes desde el archivo descrito por file desc y los guarda en la direccion apuntada ´
    por buffer. Debe retornar la cantidad de Byte efectivamente le´ıdos desde el archivo. Esto es importante si
    nbytes es mayor a la cantidad de Byte restantes en el archivo. La lectura de read se efectua desde
    la posición del archivo inmediatamente posterior a la ultima posici ´ on le ´ ´ıda por un llamado a read */

    //int byte_read = 0;  //valor de retorno, cantidad de byte efectivamente leidos.
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
    //return byte_read;
}


int cr_write(crFILE* file_desc, void* buffer, int nbytes)
{
    /*Funcion para escribir archivos. Escribe en el archivo descrito por file desc los nbytes que se encuentren
    en la direccion indicada ´
    por buffer. Retorna la cantidad de Byte escritos en el archivo. Si se produjo un error porque no pudo seguir
    escribiendo, ya sea porque el disco se lleno o porque el archivo no puede crecer m ´ as, este n ´ umero puede
    ser menor a nbytes (incluso 0) */
    /*
    printf("EL BUFFER ES: %s\n",buffer);
    int bytes_write = 0;
    int escritura_directa = 0;
    int escritura_indirecta = 0;
    //bloque_datos* BD = malloc(sizeof(bloque_datos));
    //bloque_indice* BI = malloc(sizeof(bloque_indice));
    //BI -> bloq_dat = BD;
    //BI -> entrada = file_desc -> posicion_en_bloque;
    //printf("1\n");
    //BI -> bloque = file_desc -> bloque;
    //printf("1\n");
    //BD -> offset_datos = 0;
    //printf("1\n");
    if(file_desc -> modo == 1)
    {
        int bloque = file_desc -> bloque;
        if(file_desc -> posicion_en_bloque < 1008) //reviso si queda espacio por escribir
        {
            //printf("2\n");
            if(nbytes - (258048 - (file_desc -> posicion_en_bloque)*1024) < 0) //reviso si voy a escribir todo o no ---- SI CABE TODO EL NBYTES
            {
                escritura_directa = nbytes;
            }
            else
            {
                 escritura_directa = 258048 - file_desc -> posicion_en_bloque;
                 escritura_indirecta = nbytes - (258048 - file_desc -> posicion_en_bloque);
                 file_desc -> uso_dir1 = 1;
                 if(escritura_indirecta > 262144) file_desc -> uso_dir2 = 1;
                 if(escritura_indirecta > 67108864) file_desc -> uso_dir3 = 1;
            }
        }
        else escritura_indirecta = nbytes;
        //printf("3\n");
        while(1)
        {
            FILE* archivo = fopen(DISK_PATH, "rb");
            fseek(archivo, bloque*1024 + file_desc -> posicion_en_bloque, SEEK_SET);
            unsigned char* puntero = malloc(4*(sizeof(unsigned char)));
            fread(puntero,sizeof(unsigned char),4,archivo);
            int bloque_a_ir = encontrar_bloque(puntero);
            //BD -> bloque = bloque_a_ir;
            fclose(archivo);
            printf("4\n");
            archivo = fopen(DISK_PATH, "w");
            //fseek(archivo, BD->bloque*1024 + BD -> offset_datos, SEEK_SET); //voy a leer en entrada en la que voy.
            int escritura_en_datos;
            if(escritura_directa > 1024)
            {
                escritura_en_datos = 1024;
                escritura_directa -= 1024;
            }
            else
            {
                escritura_en_datos = escritura_directa;
                escritura_directa = 0;
            }
            if(escritura_directa > 0) //si hay escritura directa
            {

                //inicialmente hago solo escritura directa
                fwrite(buffer, 1, escritura_directa, archivo); //escribo los directos
                bytes_write+=escritura_en_datos;
                if(escritura_en_datos == 1024)
                {
                    //BD -> offset_datos = 0;
                    file_desc -> posicion_en_bloque += 1;
                    //BI -> entrada += 1;
                    if(!escritura_indirecta) break;

                }
                //else BD -> offset_datos += escritura_en_datos;
                //file_desc -> posicion_en_bloque += escritura_directa;
                if(escritura_indirecta && escritura_directa == 0) //si hay indirecta y ya escribi todo lo directo
                {
                    break;
                }

            }
        }



    }
    return bytes_write;  */


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

    int tipo = objective_kind(orig);
    printf("\n %d Es el tipo\n", tipo);
    if( tipo== 4){
        char* directorios = directorio_a_agregar(dest);
        create_local_directory(directorios);
        crFILE* file = malloc(sizeof(crFILE));
    	file = cr_open(orig,'r');
    	int count = 0;
    	//printf("%d....\n",file->num_bloques);
    	uint8_t * buffer = malloc(file->tamano * sizeof(uint8_t));
    	//cr_read( file, buffer, 5000);
    	int lee = cr_read(file, buffer, file->tamano);
    	FILE* add = fopen(dest, "wb");
    	fwrite(buffer, sizeof(uint8_t), file->tamano, add);
    }
    if(tipo == 1){
        printf("%s\n", FUT1);
    }
    else{
        printf("%s\n", ERROR23);
    }
}


int cr_load(char* orig)
{
    /*Funcion que se encarga de copiar un archivo o ´ arbol de directorios, referen- ´
    ciado por orig al disco. En caso de que un archivo sea demasiado pesado para el disco, se debe escribir todo
    lo posible hasta acabar el espacio disponible */

}
