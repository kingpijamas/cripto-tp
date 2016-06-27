#ifndef MARSHALL_H
#define MARSHALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wav_header_utils.h" // FIXME

#define MAX_EXT_LEN 10

int marshall_plain(char * filename, char ** marshalled_data);
int marshall_encrypted(char * filename, char ** marshalled_data);

#endif
