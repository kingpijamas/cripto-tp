#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "define.h"

int read_file(char ** buffer, char * filename);

#endif // FILE_UTILS_H
