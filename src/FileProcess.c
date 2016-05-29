//
//  FileProcess.c
//  
//
//  Created by Julieta Sal-lari on 28/5/16.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "WavHeaderUtils.h"
#include "LSB.c"
#include "WavHeaderUtils.c"
#include "FileProcess.h"

typedef unsigned long DWORD;

void recover(FILE *fileptr, FILE *img_out){
    //primero leo el header: 44B
    struct WAV_HEADER header = parseHeader(fileptr);
    
    recoverLSB(fileptr, img_out, header.bits_per_sample/8);
}

int main(){
    
    FILE *fileptr = fopen("../../sample1.wav", "rb");  // Open the file in binary mode
    if(fileptr == NULL){
        printf("../../fileptr is NULL \n");
        return 0;
    }
    
    FILE *img = fopen("../../img1.gif", "rb");
    if(img == NULL){
        printf("img is NULL \n");
        return 0;
    }
    //me guardo el size de la imagen sz
    fseek(img, 0L, SEEK_END);
    DWORD sz = ftell(img);
    sz += sizeof(DWORD);
    rewind(img);
    
    //archivo de audio resuktante que deberia ser igual al original
    FILE * outfile = fopen("../../outfile.wav", "wb");
    if(outfile == NULL){
        printf("out is NULL \n");
        return 0;
    }
    
    //primero leo el header: 44B
    struct WAV_HEADER header = parseHeader(fileptr);
    fwrite(&header,1,sizeof(header),outfile);
    
    unsigned short int sample_size = header.bits_per_sample / 8;
    
    hideLSB(fileptr, outfile, img, sz, sample_size);
    
    recover(fileptr, img);
    
    fclose(img);
    fclose(fileptr);
    fclose(outfile);
    
    return 0;
}
