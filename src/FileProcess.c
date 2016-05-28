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
#include "Encode.h"
#include "WavHeaderUtils.h"
#include "Encode.c"
#include "WavHeaderUtils.c"
#include "FileProcess.h"

int main(){
    
    FILE *fileptr = fopen("../../sample1.wav", "rb");  // Open the file in binary mode
    if(fileptr == NULL){
        printf("../../fileptr is NULL \n");
        return 0;
    }
    
    FILE *img = fopen("../../prueba.png", "rb");
    if(img == NULL){
        printf("img is NULL \n");
        return 0;
    }
    
    //archivo de audio resuktante que deberia ser igual al original
    FILE * outfile = fopen("../../outfile.wav", "wb");
    if(outfile == NULL){
        printf("out is NULL \n");
        return 0;
    }
    int write = 0;
    
    //primero leo el header: 44B
    struct WAV_HEADER header = parseHeader(fileptr);
    
    fwrite(&header,1,sizeof(header),outfile);
    
    FILE *img_out = fopen("../../img_out.png", "wb");  // Open the file in binary mode
    if(img_out == NULL){
        printf("../../fileptr is NULL \n");
        return 0;
    }
    
    int read = 0;
    int img_read = 0;
    unsigned short int sample_size = header.bits_per_sample / 8;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);
    unsigned char img_buffer[1];
    unsigned char bit;
    int index = 0;
    //leo el primer B de la imagen
    img_read = fread(img_buffer, 1, 1, img);
    unsigned char imgByte = 0;
    //leo un sample a la vez
    while((read = fread(buffer, 1, sample_size, fileptr)) > 0)
    {
        
        if (index>=8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            img_read = fread(img_buffer, 1, 1, img);
        }
        if (img_read > 0) {
            //agarro el bit index
            bit = (img_buffer[0]>> index) & 1;
            
            //cambio el ultimo bit del buffer de lectura
            encode(buffer, read, bit);
            index++;
        }
        fwrite(buffer,1,read,outfile);			// Writing read data into output file
        
        if (write < 0) {
            printf("Error writing headers\n");
            return -1;
        }
        
    }
    fclose(img);
    fclose(fileptr);
    fclose(outfile);
    
    return 0;
}