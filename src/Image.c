//
//  Image.c
//  
//
//  Created by Julieta Sal-lari on 28/5/16.
//
//
#include "Image.h"

int main(){
    FILE *img = fopen("prueba.png", "rb");  // Open the file in binary mode
    if(img == NULL){
        printf("../../fileptr is NULL \n");
        return 0;
    }
    
    FILE *img_out = fopen("img_out.png", "wb");  // Open the file in binary mode
    if(img_out == NULL){
        printf("../../fileptr is NULL \n");
        return 0;
    }
    
    int read = 0;
    int img_read = 0;
    unsigned char buffer[1];
    unsigned char img_buffer[1];
    unsigned char bit;
    int index = 0;
    //leo el primer B de la imagen
    img_read = fread(buffer, 1, 1, img);
    img_buffer[0] = 0;
    while(img_read > 0)
    {
        if (index>=8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            img_read = fread(buffer, 1, 1, img);
            fwrite(img_buffer,1,1,img_out);
            img_buffer[0] = 0;
        }
        if (img_read > 0) {
            bit = (buffer[0] >> index) & 1;
            
        }
        if ((bit & 0x01) == 1) {
            img_buffer[0] |= 1 << index;
        }
        index++;
    }
    
    fclose(img);
    fclose(img_out);
    return 0;
}