//
//  LSB4.h
//  
//
//  Created by Julieta Sal-lari on 29/5/16.
//
//

#ifndef LSB4_H
#define LSB4_H

#include <stdio.h>
#include "WavHeaderUtils.h"

void hideLSB4(FILE *fileptr, FILE *outfile, unsigned char *img, DWORD sz, unsigned short int sample_size);
#endif // LSB4_H
