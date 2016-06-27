#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "define.h"

int read_file(char ** buffer, char * filename);
int create_file(char * base_path, char * extension, char * data, int data_size);

#endif // FILE_UTILS_H
