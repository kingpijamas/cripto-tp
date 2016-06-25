//
//  LSB.h
//
//
//  Created by Julieta Sal-lari on 29/5/16.
//
//

#ifndef LSB1_H
#define LSB1_H

#include "define.h"

void hideLSB1(FILE *fileptr, FILE *outfile, unsigned char *img, DWORD sz, unsigned short int sample_size);
void recoverLSB1(FILE *fileptr, FILE *img_out, unsigned short int sample_size);

#endif // LSB1_H
