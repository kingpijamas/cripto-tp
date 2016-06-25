//
//  Image.c
//
//
//  Created by Julieta Sal-lari on 28/5/16.
//
//
#include "../include/image.h"

//void recover(){
//    FILE *fileptr = fopen("../../outfile.wav", "rb");  // Open the file in binary mode
//    if(fileptr == NULL){
//        printf("../../fileptr is NULL \n");
//        return;
//    }
//
//    FILE *img_out = fopen("../../img_out.png", "wb");  // Open the file in binary mode
//    if(img_out == NULL){
//        printf("../../img_out is NULL \n");
//        return;
//    }
//
//    //primero leo el header: 44B
//    struct WAV_HEADER header = parseHeader(fileptr);
//    if (header.length_of_fmt == 16) {
//        //daltan leer  bytes vacios
//        unsigned char aux_buffer[2];
//        fread(aux_buffer, 1, 2, fileptr);
//        fwrite(aux_buffer, 1, 2, img_out);
//    }
//
//    int read = 0;
//    int img_read = 0;
//    unsigned short int sample_size = header.bits_per_sample / 8;
//    unsigned char *buffer = (unsigned char *)malloc(sample_size);
//    unsigned char img_buffer[1];
//    unsigned char bit;
//    int index = 0;
//    //leo el primer B de la imagen
//    img_read = fread(img_buffer, 1, 1, fileptr);
//    unsigned char imgByte = 0;
//    //leo un sample a la vez
//    int size = 824;
//    int total = 0;
//    img_buffer[0] = 0;
//    while(total < size)
//    {
//        read = fread(buffer, 1, sample_size, fileptr);
//        if (index>=8) { //si ya lei todo un byte agarro el que sigue
//            index = 0;
//            img_read = fwrite(img_buffer, 1, 1, img_out);
//            img_buffer[0] = 0;
//        }
//        if (read > 0) {
//            //agarro el bit index
//            bit = ((buffer[sample_size-1]) >> 0) & 1;
//            if ((bit & 0x01) == 1) {
//                img_buffer[0] |= 1 << index;
//            }
//            index++;
//        }
//
//    }
//    printf("%d \n", total);
//    fclose(img_out);
//    fclose(fileptr);
//}

/* int main(){
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
*/
