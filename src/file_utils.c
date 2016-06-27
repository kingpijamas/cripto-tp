#include "../include/file_utils.h"

int read_file(char ** buffer, char * filename) {
	DWORD length = -1;
	FILE * file = fopen(filename, "rb");

	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		*buffer = (char *) malloc(length);
		if (*buffer) {
			fread(*buffer, 1, length, file);
		}
		fclose(file);
	}
	return length;
}

int create_file(char * base_path, char * extension, char * data, int data_size) {
	char * filename = (char *) calloc(strlen(base_path) + strlen(extension) + 1, sizeof(char));
	sprintf(filename, "%s%s", base_path, extension);

	FILE * data_file = fopen(filename, "wb+");
	int bytes_written = fwrite(data, data_size, 1, data_file);

	free(filename);
	return bytes_written;
}
