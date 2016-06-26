#ifndef MARSHALL_H
#define MARSHALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wav_header_utils.h" // FIXME

char * marshall_plain(char * filename);
char * marshall_encrypted(char * filename);

#endif
