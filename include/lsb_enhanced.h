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

void hide_lsb_enh(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, int bytes_to_hide);

int recover_lsb_enh(FILE * data_file, FILE * vector, unsigned short int sample_bytes);

#endif // LSBENHANCED_H
