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
    
    FILE *fileptr = fopen("../../sample1.wav", "r+");  // Open the file in binary mode
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
        
    //primero leo el header: 44B
    struct WAV_HEADER header = parseHeader(fileptr);

    fseek( fileptr, -(sizeof(header)), SEEK_CUR );
    fwrite(&header,1,sizeof(header),fileptr);
    
    unsigned short int sample_size = header.bits_per_sample / 8;
    
    hideLSB(fileptr, img, sz, sample_size);
    
    fclose(img);
    fclose(fileptr);
    
    
    FILE *outfile = fopen("../../sample1.wav", "rb");
    if(outfile == NULL){
        printf("out is NULL \n");
        return 0;
    }
    
    FILE *img_out = fopen("../../img_out1.gif", "wb");
    if(img == NULL){
        printf("img is NULL \n");
        return 0;
    }
    
    recover(outfile, img_out);
    
    fclose(outfile);
    fclose(img_out);
    
    return 0;
}
