//
//  WavHeaderUtils.c
//  
//
//  Created by Julieta Sal-lari on 25/5/16.
//
//
#include <math.h>
#include "WavHeaderUtils.h"

void parse(struct WAV_HEADER *header){
    
    printf("(41-44) Size of data chunk: %u \n", header->data_size);
}

void loadImg(){
    FILE *fileptr = fopen("outfile.wav", "rb");  // Open the file in binary mode
    if(fileptr == NULL){
        printf("fileptr is NULL \n");
        return;
    }
    FILE * outfile = fopen("imgfile.bmp", "wb");
    if(outfile == NULL){
        printf("out is NULL \n");
        return;
    }
    int write = 0;
    
    struct WAV_HEADER header;
    memset(&header, 0, sizeof(header));
    int read = fread(&header,sizeof(header),1,fileptr);
    if (read <= 0) {
        printf("hola!");
        return;
    }
    
    read = 0;
    int img_read = 0;
    unsigned short int sample_size = header.bits_per_sample / 8;
    unsigned char buffer[sample_size];
    unsigned char img_buffer[1];
    int index = 0;
    unsigned char bit = 0x00;
    img_buffer[0] = 0;
    while((read = fread(buffer, 1, sample_size, fileptr)) > 0)
    {
        if (index==7) {
            index = 0;
            //img_buffer[1] = bit;
            fwrite(img_buffer,1,1,outfile);
            if (write < 0) {
                printf("Error writing headers\n");
            }
            img_buffer[0] = 0;
        }
        if ((buffer[sample_size-1] & 0x01) == 1) {
            img_buffer[0] += pow(2, index) * 1;
        }else{
            img_buffer[0] += 0;
        }
        
        index++;
    }

    fclose(fileptr);
    fclose(outfile);
}

int main(){

    FILE *fileptr = fopen("sample1.wav", "rb");  // Open the file in binary mode
    if(fileptr == NULL){
        printf("fileptr is NULL \n");
        return 0;
    }
    
    FILE *img = fopen("Alfred.bmp", "rb");
    if(img == NULL){
        printf("img is NULL \n");
        return 0;
    }
    
    FILE * outfile = fopen("outfile.wav", "wb");
    if(outfile == NULL){
        printf("out is NULL \n");
        return 0;
    }
    int write = 0;
    
    struct WAV_HEADER header;
    memset(&header, 0, sizeof(header));
    int read = fread(&header,sizeof(header),1,fileptr);
    if (read <= 0) {
        printf("hola!");
    }
    fwrite(&header,1,sizeof(header),outfile);
    if (write < 0) {
        printf("Error writing headers\n");
        return -1;
    }
    parse(&header);
    
    read = 0;
    int img_read = 0;
    unsigned short int sample_size = header.bits_per_sample / 8;
    unsigned char buffer[sample_size];
    unsigned char img_buffer[1];
    unsigned char bit;
    int index = 0;
    img_read = fread(img_buffer, 1, 1, img);
    
    while((read = fread(buffer, 1, sample_size, fileptr)) > 0)
    {
        if (index>=8) {
            index = 0;
            img_read = fread(img_buffer, 1, 1, img);
        }
        if (img_read > 0) {
            
            bit = img_read & (1<<(index));
            index++;
            
            if (((bit & 0x01) == 1) && ((read % 2) == 0)) {
                buffer[sample_size-1]= buffer[sample_size-1] + 0x01;
                //printf("changed to 1");
            }else if (((bit & 0x01) == 0) && ((read % 2) == 1)) {
                buffer[sample_size-1] = buffer[sample_size-1] - 0x01;
                //printf("changed to 0");
            }

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
    
    loadImg();
    return 0;

}

//int parseHeader(WAV_HEADER *header, unsigned char *buffer){
//    read = fread(header.riff, sizeof(header.riff), 1, fileptr);
//    printf("(1-4): %s \n", header.riff);
//    
//    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
//    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
//    
//    header.overall_size  = littleTobigEndian4(buffer4);
//    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);
//    
//    read = fread(header.wave, sizeof(header.wave), 1, fileptr);
//    printf("(9-12) Wave marker: %s\n", header.wave);
//    
//    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker),1, fileptr);
//    printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);
//    
//    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
//    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
//    
//    header.length_of_fmt = littleTobigEndian4(buffer4);
//    printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);
//    
//    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
//    printf("%u %u \n", buffer2[0], buffer2[1]);
//    
//    header.format_type = littleTobigEndian2(buffer2);
//    printf("(21-22) Format type: %u \n", header.format_type);
//    
//    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
//    printf("%u %u \n", buffer2[0], buffer2[1]);
//    
//    header.channels = littleTobigEndian2(buffer2);
//    printf("(23-24) Channels: %u \n", header.channels);
//    
//    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
//    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
//    
//    header.sample_rate = littleTobigEndian4(buffer4);
//    printf("(25-28) Sample rate: %u\n", header.sample_rate);
//    
//    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
//    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
//    
//    header.byterate = littleTobigEndian4(buffer4);
//    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate,header.byterate*8);
//    
//    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
//    printf("%u %u \n", buffer2[0], buffer2[1]);
//    
//    header.block_align = littleTobigEndian2(buffer2);
//    printf("(33-34) Block Alignment: %u \n", header.block_align);
//    
//    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
//    printf("%u %u \n", buffer2[0], buffer2[1]);
//    
//    header.bits_per_sample = littleTobigEndian2(buffer2);
//    printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);
//    
//    read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, fileptr);
//    printf("(37-40) Data Marker: %s \n", header.data_chunk_header);
//    
//    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
//    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
//    
//    header.data_size = littleTobigEndian4(buffer4);
//    printf("(41-44) Size of data chunk: %u \n", header.data_size);
//    
//
//}