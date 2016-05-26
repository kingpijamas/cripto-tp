#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//TODO capaz poner esta constante y las que se usen en un .h
#define OK 0
#define INVALID_OP 1
#define MISSING_IN 2
#define MISSING_P 3
#define INVALID_IN_FORMAT 4
#define MISSING_IN_FILE 5
#define MISSING_P_FILE 6
#define INVALID_P_FORMAT 7

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

    int error = 0;
    int embed = 0, extract = 0;
    int index;
    char *dot;
    char * inPath, * pPath;
    for(index = 1; index<argc && !error; index++){
        switch(index){
            case 1:
                if(strcmp(argv[index], "-embed") == 0){
                    embed = 1;
                }else if(strcmp(argv[index], "-extract") == 0){
                    extract = 1;
                }else{
                    printf("Operacion invalida\n");
                    error = INVALID_OP;
                }
                break;
            case 2:
                if(embed == 1){
                    if(strcmp(argv[index], "-in") != 0){
                        printf("Falta el parametro -in\n");
                        error = MISSING_IN;
                    }
                }else if(extract == 1){
                    if(strcmp(argv[index], "-p") != 0){
                        printf("Falta el parametro -p\n");
                        error = MISSING_P;
                    }
                }
                break;
            case 3:
                dot = strrchr(argv[index], '.');
                if(embed == 1){
                    if(dot && !strcmp(dot, ".bmp")){
                        //F_OK : Flag meaning test for existence of the file
                        if(access( argv[index], F_OK)==-1) {
                            printf("No existe el archivo\n");
                            error = MISSING_IN_FILE;
                        }else{
                            inPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -in espera .bmp \n");
                        error = INVALID_IN_FORMAT;
                    }
                }
                break;
            case 4:
                if(embed == 1){
                    if(strcmp(argv[index], "-p") != 0){
                        printf("Falta el parametro -p\n");
                        error = MISSING_P;
                    }
                }
                break;
            case 5:
                dot = strrchr(argv[index], '.');
                if(embed == 1){
                    if(dot && !strcmp(dot, ".wav")){
                        //F_OK : Flag meaning test for existence of the file
                        if(access( argv[index], F_OK)==-1) {
                            printf("No existe el archivo\n");
                            error = MISSING_P_FILE;
                        }else{
                            pPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -p espera .wav \n");
                        error = INVALID_P_FORMAT;
                    }
                }
        }
    }
}