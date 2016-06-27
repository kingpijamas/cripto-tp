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
#include "marshall.h"

void hide_lsb_enh(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, int bytes_to_hide);
int recover_lsb_enh(char * base_out_path, FILE * vector, unsigned short int sample_bytes);

#endif // LSBENHANCED_H
