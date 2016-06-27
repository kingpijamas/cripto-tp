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

void hide_lsb4(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, int bytes_to_hide);
int recover_lsb4(FILE * data_file, FILE * vector, unsigned short int sample_bytes);

#endif // LSB4_H
