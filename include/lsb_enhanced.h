//
//  LSBEnhanced.h
//
//
//  Created by Julieta Sal-lari on 30/5/16.
//
//

#ifndef LSBENHANCED_H
#define LSBENHANCED_H

#include "define.h"

void hide_lsb_enh(FILE *fileptr, FILE *outfile, unsigned char *img, DWORD sz, unsigned short int sample_size);

void recover_lsb_enh(FILE *fileptr, FILE *img_out, unsigned short int sample_size);

#endif // LSBENHANCED_H
