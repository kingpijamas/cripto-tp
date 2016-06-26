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

void hide_lsb1(FILE * vector, FILE * in_file, unsigned short int sample_bytes, char * data);
int recover_lsb1(FILE * data_file, FILE * vector, unsigned short int sample_bytes);

#endif // LSB1_H
