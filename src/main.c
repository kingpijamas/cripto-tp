#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TODO capaz poner esta constante y las que se usen en un .h
#define OK 0

int
main(int argc, char **argv)
{
	if(argc == 2 && strcmp(argv[1], "-h") == 0){
		printf("----------------------------------------------------------------------------------------------\n");
        printf("Help Menu \n");
        printf("----------------------------------------------------------------------------------------------\n");
        printf("-embed \t\t\t\t\t\t Indica que se va a ocultar información \n");
        printf("-extract \t\t\t\t\t Indica que se va a extraer información \n");
        printf("-in [file] \t\t\t\t\t Archivo que se va a ocultar \n");
        printf("-p [wavefile] \t\t\t\t\t Archivo wav portador \n");
       	printf("-out [file] \t\t\t\t\t Archivo de salida obtenido \n");
       	printf("-steg <LSB1 | LSB4 | LSBE> \t\t\t Algoritmo de esteganografiado \n");
		printf("-a <aes128 | aes192 | aes256 | des> (OPTIONAL) \n");
		printf("-m <ecb | cfb | ofb | cbc> (OPTIONAL) \n");
        printf("-pass [password] (OPTIONAL) \t\t\t password de encripcion \n");
        printf("----------------------------------------------------------------------------------------------\n");
        return OK;
	}

	if(argc < 9 || argc>15 ){
        printf("Numero incorrecto de parametros\n");
        return -1;
    }
}