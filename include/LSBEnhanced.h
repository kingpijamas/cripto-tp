//
//  LSBEnhanced.h
//  
//
//  Created by Julieta Sal-lari on 30/5/16.
//
//

#ifndef LSBENHANCED_H
#define LSBENHANCED_H

#include <stdio.h>
#include "WavHeaderUtils.h"

void hideLSBEnh(FILE *fileptr, FILE *outfile, unsigned char *img, DWORD sz, unsigned short int sample_size);

void recoverLSBEnh(FILE *fileptr, FILE *img_out, unsigned short int sample_size);

#endif // LSBENHANCED_H
