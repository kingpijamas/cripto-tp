#include "../include/marshall.h"

// static void print_bits(unsigned char num);
//
// void print_bits(unsigned char num) {
// 	unsigned char size = sizeof(unsigned char);
// 	unsigned char maxPow = 1 << (size * 8 - 1);
//
// 	printf("'");
// 	for (unsigned int i = 0; i < size * 8; i++) {
// 		// print last bit and shift left.
// 		printf("%u", (num & maxPow) ? 1 : 0);
// 		num <<= 1;
// 	}
// 	printf("'");
// }

int marshall_plain(char * filename, char ** marshalled_data) {
	char * extension = strrchr(filename, '.');
	char ** payload_buffer = (char **) calloc(1, sizeof(char **));

	int size_bytes = sizeof(DWORD);
	int extension_bytes = strlen(extension) + 1; // \0 has to be marshalled
	DWORD payload_bytes = read_file(payload_buffer, filename);
	if (payload_bytes == -1) {
		return -1;
	}
	int marshalled_size = size_bytes + payload_bytes + extension_bytes;
	*marshalled_data = (char *) calloc(marshalled_size, sizeof(char));

	DWORD flipped_payload_bytes = __builtin_bswap64(payload_bytes);

	memcpy(*marshalled_data, &flipped_payload_bytes, size_bytes);
	memcpy((*marshalled_data) + size_bytes, *payload_buffer, payload_bytes);
	memcpy((*marshalled_data) + size_bytes + payload_bytes, extension, extension_bytes);

	free(*payload_buffer);
	free(payload_buffer);
	return marshalled_size;
}

int marshall_encrypted(char * filename, char ** marshalled_data) {
	char ** payload_buffer = (char **) calloc(1, sizeof(char **));

	int size_bytes = sizeof(DWORD);
	DWORD payload_bytes = read_file(payload_buffer, filename);
	if (payload_bytes == -1) {
		return -1;
	}
	int marshalled_size = size_bytes + payload_bytes;
	*marshalled_data = (char *) calloc(marshalled_size, sizeof(char));

	DWORD flipped_payload_bytes = __builtin_bswap64(payload_bytes);

	memcpy(*marshalled_data, &flipped_payload_bytes, size_bytes);
	memcpy((*marshalled_data) + size_bytes, *payload_buffer, payload_bytes);

	free(*payload_buffer);
	free(payload_buffer);
	return marshalled_size;
}
