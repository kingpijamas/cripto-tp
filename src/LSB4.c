//
//  LSB4.c
//
//
//  Created by Julieta Sal-lari on 29/5/16.
//
//

#include "LSB4.h"

typedef unsigned long DWORD;

void hideBitsLSB4(unsigned char *buffer, int size, unsigned char img_bit[]){
    if(img_bit[0] == 1){
        buffer[size-1] |= 1;
    }else{
        buffer[size-1] &= ~1;
    }
    
    if(img_bit[1] == 1){
        buffer[size-1] |= 2;
    }else{
        buffer[size-1] &= ~2;
    }
    
    if(img_bit[2] == 1){
        buffer[size-1] |= 4;
    }else{
        buffer[size-1] &= ~4;
    }
    
    if(img_bit[3] == 1){
        buffer[size-1] |= 8;
    }else{
        buffer[size-1] &= ~8;
    }
    return;
}

DWORD getSizeLSB4(FILE *fileptr, unsigned short int sample_size){
    unsigned char buffer[sample_size];
    int dword_size = sizeof(DWORD);
    unsigned char *bytes = (unsigned char *)malloc(dword_size);
    
    DWORD size = 0;
    int index = 0;
    int read = 0;
    unsigned char bits[4];
    int i = 0;
    while (i < dword_size) {
        if (index == 0 ) {
            bytes[i] = 0;
        }
        read = fread(buffer, 1, sample_size, fileptr);
        //agarro el bit menos sig de la musetra
        bits[0] = ((buffer[sample_size-1])>> 0) & 1;
        bits[1] = ((buffer[sample_size-1])>> 1) & 1;
        bits[2] = ((buffer[sample_size-1])>> 2) & 1;
        bits[3] = ((buffer[sample_size-1])>> 3) & 1;
        
        if ((bits[0] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        if ((bits[1] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        if ((bits[2] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        if ((bits[3] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        
        if (index%8 == 0) { //si ya lei todo un byte incremento el tamaño
            i++;
        }
    }
    free(bytes);
    return size;
}

void insertSizeLSB4(FILE *fileptr, FILE *outfile, unsigned short int sample_size, DWORD size){
    //en cuantos bytes se guarda el tamaño
    int dword_size = sizeof(size);
    
    unsigned char *bytes = (unsigned char*)malloc(dword_size);
    memcpy(bytes, &size, dword_size);
    
    int index = 0;
    int read = 0;
    unsigned char byte = bytes[0];  //agarro el primer byte del tamaño
    unsigned char bits[4];
    unsigned char *buffer = (unsigned char *)malloc(sample_size);
    
    int i = 0;
    while (i < dword_size) {
        read = fread(buffer, 1, sample_size, fileptr);
        //agarro el bit index
        bits[0] = (byte>> index++) & 1;
        bits[1] = (byte>> index++) & 1;
        bits[2] = (byte>> index++) & 1;
        bits[3] = (byte>> index++) & 1;
        //printf("%u %u %u %u", bits[0], bits[1], bits[2], bits[3]);
        //cambio el ultimo bit del buffer de lectura
        hideBitsLSB4(buffer, read, bits);
        
        fwrite(buffer,1,read,outfile);			// Writing read data into output file
        
        if (index>=8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            byte = bytes[++i];
        }
    }
    printf("%lu \n", size);
    free(bytes);
    free(buffer);
}
void hideLSB4(FILE *fileptr, FILE *outfile, FILE *img, DWORD sz, unsigned short int sample_size){
    int read = 0;
    int write = 0;
    int img_read = 0;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);   //leo de a samples
    unsigned char *img_buffer = (unsigned char *)malloc(1);         //leo de a un byte
    unsigned char bits[4];
    int index = 0;
    //inserto el size
    insertSizeLSB4(fileptr, outfile, sample_size, sz);
    
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
            bits[0] = (img_buffer[0]>> index++) & 1;
            bits[1] = (img_buffer[0]>> index++) & 1;
            bits[2] = (img_buffer[0]>> index++) & 1;
            bits[3] = (img_buffer[0]>> index++) & 1;
            
            //cambio el ultimo bit del buffer de lectura
            //printf("%u %u %u %u", bits[0], bits[1], bits[2], bits[3]);
            hideBitsLSB4(buffer, read, bits);
            
        }
        fwrite(buffer,1,read,outfile);			// Writing read data into output file
        
    }
}

void recoverLSB4(FILE *fileptr, FILE *img_out,unsigned short int sample_size){
    int read = 0;
    int img_read = 0;
    //unsigned short int sample_size = header.bits_per_sample / 8;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);
    unsigned char img_buffer[1];
    unsigned char bits[4];
    int index = 0;
    
    img_buffer[0] = 0;
    
    
    DWORD size = getSizeLSB4(fileptr, sample_size);
    
    while(size > 0)
    {
        read = fread(buffer, 1, sample_size, fileptr);
        
        if (index>=8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            img_read = fwrite(img_buffer, 1, 1, img_out);
            img_buffer[0] = 0;
            size--;
        }
        //agarro el ultimo bit de la muestra
        bits[0] = ((buffer[sample_size-1]) >> 0) & 1;
        bits[1] = ((buffer[sample_size-1]) >> 1) & 1;
        bits[2] = ((buffer[sample_size-1]) >> 2) & 1;
        bits[3] = ((buffer[sample_size-1]) >> 3) & 1;
        
        if ((bits[0] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
        if ((bits[1] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
        if ((bits[2] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
        if ((bits[3] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
    }
}