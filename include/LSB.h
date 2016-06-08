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
typedef unsigned long DWORD;


void insertSizeLSB(FILE *fileptr, FILE *outfile, unsigned short int sample_size, DWORD size);

DWORD getSizeLSB(FILE *fileptr, unsigned short int sample_size);

void hideBit(unsigned char *buffer, int size, unsigned char img_bit);
#endif // LSB_H
