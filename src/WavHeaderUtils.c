//
//  WavHeaderUtils.c
//  
//
//  Created by Julieta Sal-lari on 25/5/16.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include "WavHeaderUtils.h"

struct WAV_HEADER parseHeader(FILE * fileptr);


unsigned int littleTobigEndian4(unsigned char buffer4[]){
    return buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
}

unsigned int littleTobigEndian2(unsigned char buffer2[]) {
    return buffer2[0] | (buffer2[1] << 8);
}

struct WAV_HEADER parseHeader(FILE * fileptr){
    struct WAV_HEADER header;
    memset(&header, 0, sizeof(header));
    
    unsigned char buffer2[2];
    unsigned char buffer4[4];
    
    int read = fread(header.riff, sizeof(header.riff), 1, fileptr);
    printf("(1-4): %s \n", header.riff);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.overall_size  = littleTobigEndian4(buffer4);
    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);
    
    read = fread(header.wave, sizeof(header.wave), 1, fileptr);
    printf("(9-12) Wave marker: %s\n", header.wave);
    
    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker),1, fileptr);
    printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.length_of_fmt = littleTobigEndian4(buffer4);
    printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.format_type = littleTobigEndian2(buffer2);
    printf("(21-22) Format type: %u \n", header.format_type);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.channels = littleTobigEndian2(buffer2);
    printf("(23-24) Channels: %u \n", header.channels);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.sample_rate = littleTobigEndian4(buffer4);
    printf("(25-28) Sample rate: %u\n", header.sample_rate);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.byterate = littleTobigEndian4(buffer4);
    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate,header.byterate*8);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.block_align = littleTobigEndian2(buffer2);
    printf("(33-34) Block Alignment: %u \n", header.block_align);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.bits_per_sample = littleTobigEndian2(buffer2);
    printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);
    
    read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, fileptr);
    printf("(37-40) Data Marker: %s \n", header.data_chunk_header);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.data_size = littleTobigEndian4(buffer4);
    printf("(41-44) Size of data chunk: %u \n", header.data_size);
    
    return header;
}

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
    
    int read = 0;
    int img_read = 0;
    unsigned short int sample_size = header.bits_per_sample / 8;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);
    unsigned char img_buffer[1];
    unsigned char bit;
    int index = 0;
    //leo el primer B de la imagen
    img_read = fread(img_buffer, 1, 1, img);
    
    //leo un sample a la vez
    while((read = fread(buffer, 1, sample_size, fileptr)) > 0)
    {
        
        if (index>=8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            img_read = fread(img_buffer, 1, 1, img);
        }
        if (img_read > 0) {
            //printf("%u %u", *buffer, *(buffer+sample_size-1));
            //agarro el bit index
            bit = (*img_buffer) & (1<<(index));
            index++;
            
            //cambio el ultimo bit del buffer de lectura
            if (((bit & 0x01) == 1) && (((*(buffer+sample_size-1))  & 0x01 ) == 0)) {
                //(*(buffer))= (*(buffer+read-1)) | 0x01;
                //printf("changed to 1");
            }else if (((bit & 0x01) == 0) && (((*(buffer+sample_size-1)) & 0x01 )== 1)) {
                //(*(buffer+read-1)) = (*(buffer+read-1)) & 0xfe;
                //printf("changed to 0");
            }
            //printf(" - %u %u\n", *buffer, *(buffer+sample_size-1));
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