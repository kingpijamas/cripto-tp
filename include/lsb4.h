#ifndef LSB4_H
#define LSB4_H

#include "define.h"
#include "marshall.h"
#include "file_utils.h"

void hide_lsb4(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, int bytes_to_hide);
int recover_lsb4(char * base_out_path, FILE * vector, unsigned short int sample_bytes);

#endif // LSB4_H
