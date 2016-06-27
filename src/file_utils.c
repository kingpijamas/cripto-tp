#include "../include/file_utils.h"

int read_file(char ** buffer, char * filename) {
	DWORD length = -1;
	FILE * file = fopen(filename, "rb");

	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		*buffer = (char *) malloc(length + 1);
		if (*buffer) {
			fread(*buffer, 1, length, file);
			(*buffer)[length] = '\0';
		}
		fclose(file);
	}
	return length;
}
