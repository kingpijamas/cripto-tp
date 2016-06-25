//
//  LSB.h
//  
//
//  Created by Julieta Sal-lari on 29/5/16.
//
//

#ifndef LSB_H
#define LSB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned long DWORD;


void insertSizeLSB1(FILE *fileptr, FILE *outfile, unsigned short int sample_size, DWORD size);

DWORD getSizeLSB1(FILE *fileptr, unsigned short int sample_size);

void hideBit(unsigned char *buffer, int size, unsigned char img_bit);

void hideLSB1(FILE *fileptr, FILE *outfile, unsigned char *img, DWORD sz, unsigned short int sample_size);

void recoverLSB1(FILE *fileptr, FILE *img_out, unsigned short int sample_size);

#endif // LSB_H
