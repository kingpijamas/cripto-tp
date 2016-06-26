#include "../include/marshall.h"

static int read_file(char ** buffer, char * filename);
static void print_bits(unsigned char num);

void print_bits(unsigned char num) {
	unsigned char size = sizeof(unsigned char);
	unsigned char maxPow = 1 << (size * 8 - 1);

	printf("'");
	for (unsigned int i = 0; i < size * 8; i++) {
		// print last bit and shift left.
		printf("%u", (num & maxPow) ? 1 : 0);
		num <<= 1;
	}
	printf("'");
}

char * marshall_plain(char * filename) {
	char * extension = strrchr(filename, '.');
	char ** payload_buffer = (char **) calloc(1, sizeof(char **));

	int size_bytes = sizeof(DWORD);
	int extension_bytes = strlen(extension) + 1; // \0 has to be marshalled
	DWORD payload_bytes = read_file(payload_buffer, filename);
	if (payload_bytes == -1) {
		return NULL;
	}
	int marshalled_size = size_bytes + payload_bytes + extension_bytes;
	char * marshalled_data = (char *) calloc(1, marshalled_size);

	DWORD test = __builtin_bswap64(payload_bytes);

	memcpy(marshalled_data, &test, size_bytes);
	memcpy(marshalled_data + size_bytes, *payload_buffer, payload_bytes);
	memcpy(marshalled_data + size_bytes + payload_bytes, extension, extension_bytes);

	printf("marshalled_data:\n  size: %lu ", marshalled_data[0]);
	print_bits(marshalled_data[0]);
	printf("\n- %d", 0);
	printf("\n  payload: '%s'", marshalled_data + size_bytes);
	printf("\n- %d", size_bytes);
	printf("\n  extension: '%s'", marshalled_data + size_bytes + payload_bytes);
	printf("\n- %d", payload_bytes + size_bytes);

	for (int i = 0; i < marshalled_size; i++) {
		printf("\n(%d)", i);
		print_bits(marshalled_data[i]);
	}
	printf("\n\n");

	free(*payload_buffer);
	free(payload_buffer);
	return marshalled_data;
}

char * marshall_encrypted(char * filename) {
	char ** payload_buffer = (char **) malloc(sizeof(char **));

	int payload_bytes = read_file(payload_buffer, filename);
	if (payload_bytes == -1) {
		return NULL;
	}
	int size_bytes = sizeof(DWORD);
	int marshalled_size = size_bytes + payload_bytes + 1; // \0 has to be marshalled

	char * marshalled_data = (char *) calloc(1, marshalled_size);
	sprintf(marshalled_data, "%.*lu%s", size_bytes, (DWORD) payload_bytes, *payload_buffer);

	free(*payload_buffer);
	free(payload_buffer);
	return marshalled_data;
}

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
		printf("\nlength: %d\nbuffer: %s\n", length, *buffer);
		fclose(file);
	}
	return length;
}
