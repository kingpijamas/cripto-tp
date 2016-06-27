#ifndef STEGOANALISIS_H
#define STEGOANALISIS_H

#include "define.h"
#include "marshall.h"

void hide_lsb1(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data,
		unsigned int bytes_to_hide);
int recover_lsb1(char * file_name, FILE * vector, unsigned short int sample_bytes);

#endif // STEGOANALISIS_H
