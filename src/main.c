#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/define.h"

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
    char * inPath; //archivo que se va a ocultar
    char * pPath; //archivo que sera el portador
    char * outPath; //archivo de salida
    StegType steg; //algoritmo de esteganografiado
    EncType enc; //encriptacion

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
                    //TODO chequear esto porque no necesariamente es .bmp
                    if(dot /*&& !strcmp(dot, ".bmp")*/){
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
            case 6:
                if(embed == 1){
                    if(strcmp(argv[index], "-out") != 0){
                        printf("Falta el parametro -out\n");
                        error = MISSING_OUT;
                    }
                }
                break;
            case 7:
                dot = strrchr(argv[index], '.');
                if(embed == 1){
                    if(dot && !strcmp(dot, ".wav")){
                        //F_OK : Flag meaning test for existence of the file
                        if(access( argv[index], F_OK)==-1) {
                            printf("No existe el archivo\n");
                            error = MISSING_OUT_FILE;
                        }else{
                            outPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -out espera .wav \n");
                        error = INVALID_OUT_FORMAT;
                    }
                }
            case 8:
                if(embed == 1){
                    if(strcmp(argv[index], "-steg") != 0){
                        printf("Falta el parametro -steg\n");
                        error = MISSING_STEG;
                    }
                }
                break;
            // case 9:
            //     if(embed == 1){
            //         switch(argv[index]){
            //             case LSB1: case LSB4: case LSBE:
            //                 steg = argv[index];
            //                 break;
            //             default:
            //                 printf("No es valido el algoritmo de esteganografiado \n");
            //                 error = INVALID_STEG;
            //                 break;
            //         }
            //     }
            //     break;
            // default:
            //     if(embed == 1){
            //         if(strcmp(argv[index], "-a") == 0){
            //             index++;
            //             switch(argv[index]){
            //             case aes128: case aes192: case aes256: case des:
            //                 enc = argv[index];
            //                 break;
            //             default:
            //                 printf("No es valida la encriptacion\n");
            //                 error = INVALID_A;
            //                 break;
            //             }
            //         }
            //     }
            //     break;
        }
        if(error){
            //TODO aca capaz se podria agregar algo mas
            return error;
        }

        //TODO aca habria que llamar al metodo correspondiente con los
        //parametros recibidos
        return OK;

    }
}