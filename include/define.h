#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wav_header_utils.h"

typedef enum {
	true, false
} bool;

typedef unsigned long DWORD;

#define BITS_PER_BYTE 8
#define when(name) case(name):;

#endif // DEFINE_H
