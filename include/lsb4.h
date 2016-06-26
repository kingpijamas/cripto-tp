//
//  lsb4.h
//
//
//  Created by Julieta Sal-lari on 29/5/16.
//
//

#ifndef LSB4_H
#define LSB4_H

#include "define.h"

void hide_lsb4(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data);

void recover_lsb4(FILE *fileptr, FILE *img_out, unsigned short int sample_size);

#endif // LSB4_H
