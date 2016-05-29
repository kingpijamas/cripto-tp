//
//  WavHeaderUtils.c
//  
//
//  Created by Julieta Sal-lari on 25/5/16.
//
//
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "WavHeaderUtils.h"

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
    //printf("(1-4): %s \n", header.riff);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    //printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.overall_size  = littleTobigEndian4(buffer4);
    //printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);
    
    read = fread(header.wave, sizeof(header.wave), 1, fileptr);
    //printf("(9-12) Wave marker: %s\n", header.wave);
    
    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker),1, fileptr);
    //printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    //printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.length_of_fmt = littleTobigEndian4(buffer4);
    //printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    //printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.format_type = littleTobigEndian2(buffer2);
    //printf("(21-22) Format type: %u \n", header.format_type);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    //printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.channels = littleTobigEndian2(buffer2);
    //printf("(23-24) Channels: %u \n", header.channels);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    //printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.sample_rate = littleTobigEndian4(buffer4);
    //printf("(25-28) Sample rate: %u\n", header.sample_rate);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    //printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.byterate = littleTobigEndian4(buffer4);
    //printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate,header.byterate*8);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    //printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.block_align = littleTobigEndian2(buffer2);
    //printf("(33-34) Block Alignment: %u \n", header.block_align);
    
    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    //printf("%u %u \n", buffer2[0], buffer2[1]);
    
    header.bits_per_sample = littleTobigEndian2(buffer2);
    //printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);
    
    read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, fileptr);
    //printf("(37-40) Data Marker: %s \n", header.data_chunk_header);
    
    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    //printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    
    header.data_size = littleTobigEndian4(buffer4);
    //printf("(41-44) Size of data chunk: %u \n", header.data_size);
    
    return header;
}