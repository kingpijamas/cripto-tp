#ifndef MARSHALL_H
#define MARSHALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wav_header_utils.h" // FIXME

char * marshallPlain(char * filename);
char * marshallEncrypted(char * filename);

#endif
