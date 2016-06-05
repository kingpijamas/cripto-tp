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

    // printf("argc: %d\n", argc);
    // int i;
    // for(i = 0; i < argc; i++){
    //     printf("argc %d : %s \n", i, argv[i]);
    // }

	if(argc < 10 || argc>16 ){
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
    //algoritmo de esteganografiado, encriptacion, modo y password
    char * stegType, * encType, * mode, * pass; 

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
                            printf("No existe el archivo %s\n", argv[index]);
                            error = MISSING_IN_FILE;
                        }else{
                            inPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -in espera .bmp \n");
                        error = INVALID_IN_FORMAT;
                    }
                }else if(extract == 1){
                    dot = strrchr(argv[index], '.');
                    if(dot && !strcmp(dot, ".wav")){
                        //F_OK : Flag meaning test for existence of the file
                        if(access( argv[index], F_OK)==-1) {
                            printf("No existe el archivo %s\n", argv[index]);
                            error = MISSING_P_FILE;
                        }else{
                            pPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -p espera .wav \n");
                        error = INVALID_P_FORMAT;
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
                            printf("No existe el archivo %s\n", argv[index]);
                            error = MISSING_P_FILE;
                        }else{
                            pPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -p espera .wav \n");
                        error = INVALID_P_FORMAT;
                    }
                }
                break;
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
                            printf("No existe el archivo %s\n", argv[index]);
                            error = MISSING_OUT_FILE;
                        }else{
                            outPath = argv[index];
                        }    
                    }else{
                        printf("Formato incorrecto. El parametro -out espera .wav \n");
                        error = INVALID_OUT_FORMAT;
                    }
                }
                break;
            case 8:
                if(embed == 1){
                    if(strcmp(argv[index], "-steg") != 0){
                        printf("Falta el parametro -steg\n");
                        error = MISSING_STEG;
                    }
                }
                break;
            case 9:
                if(embed == 1){
                    if(strcmp(argv[index], "LSB1") == 0
                        || strcmp(argv[index], "LSB4") == 0
                        || strcmp(argv[index], "LSBE") == 0){
                        stegType = argv[index];
                    }else{
                        printf("No es valido el algoritmo de esteganografiado \n");
                        error = INVALID_STEG;
                    }
                }
                break;
            default:
                if(embed == 1){
                    if(strcmp(argv[index], "-a") == 0){
                        index++;
                        if(strcmp(argv[index], "aes128") == 0
                            || strcmp(argv[index], "aes192") == 0
                            || strcmp(argv[index], "aes256") == 0
                            || strcmp(argv[index], "des") == 0){
                            encType = argv[index];
                        }else{
                            printf("No es valida la encriptacion\n");
                            error = INVALID_A;
                        }
                    }
                    else if(strcmp(argv[index], "-m") == 0){
                        index++;
                        if(strcmp(argv[index], "ecb") == 0
                            || strcmp(argv[index], "cfb") == 0
                            || strcmp(argv[index], "ofb") == 0
                            || strcmp(argv[index], "cbc") == 0){
                            mode = argv[index];
                        }else{
                            printf("No es valido el modo\n");
                            error = INVALID_M;
                        }
                    }
                    else if(strcmp(argv[index], "-pass") == 0){
                        index++;
                        if(strcmp(argv[index], "") != 0){
                            pass = argv[index];
                        }else{
                            printf("Falta la password\n");
                            error = MISSING_PASS;
                        }
                    }
                }
                break;
        }
    }
    
    if(embed == 1 && error == 0 && argc > 10){
        printf("Entra aca\n");
        //algoritmo y password pero no modo
        if(strcmp(encType, "") != 0 && strcmp(pass, "") != 0 
            && strcmp(mode, "") == 0 ){
            mode = "cbc"; //Se asume CBC por default
        }
        //modo y password pero no algoritmo
        else if(strcmp(encType, "") == 0 && strcmp(pass, "") != 0 
            && strcmp(mode, "") != 0 ){
            encType = "aes128"; //Se asume aes128 por default
        }
        //sólo password
        else if(strcmp(encType, "") == 0 && strcmp(pass, "") != 0 
            && strcmp(mode, "") == 0 ){
            mode = "cbc"; //Se asume CBC por default
            encType = "aes128"; //Se asume aes128 por default
        }
        //hay encriptacion, modo y no password -- no es valido
        else if(strcmp(encType, "") != 0 && strcmp(pass, "") == 0 
            && strcmp(mode, "") != 0){
            printf("Falta la password\n");
            error = MISSING_PASS;
        }
    }

    if(error){
        //TODO aca capaz se podria agregar algo mas
        return error;
    }

    //TODO aca habria que llamar al metodo correspondiente con los
    //parametros recibidos
    return OK;

}