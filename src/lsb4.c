#include "../include/lsb4.h"

static unsigned char lsb4(unsigned char * buffer, int buffer_bytes);

void hide_lsb4(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, int bytes_to_hide) {
	int input_bytes_read = 0;
	int bits_hidden = 0;
	unsigned char bits_to_hide;
	unsigned char * buffer = (unsigned char *) calloc(1, sizeof(char));

	printf("\ndata: %s\n\n", data);
	//leo un sample a la vez
	while ((input_bytes_read = fread(buffer, BYTE_SIZE, sample_bytes, orig_file)) > 0) {
		if (bits_hidden >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
			bits_hidden = 0;
			bytes_to_hide--;
			data++;
		}
		if (bytes_to_hide > 0) {
			// agarro el bit mas significativo
			bits_to_hide = ((*data) >> (4 - bits_hidden)) & 0x0F;
			// cambio el ultimo bit del buffer de lectura
			// hide_bit(buffer, sample_bytes, bits_to_hide);
			int last_idx = sample_bytes - 1;
			buffer[last_idx] = (buffer[last_idx] & 0xF0) | bits_to_hide;
			bits_hidden += 4;
		}
		fwrite(buffer, BYTE_SIZE, input_bytes_read, vector); // Writing read data into output file
	}
}

int recover_lsb4(char * out_path, FILE * vector, unsigned short int sample_bytes, bool ext) {
	// load body size
	DWORD data_size = 0;
	int bytes_recovered = recover_bytes_lsb4((char *) &data_size, vector, sample_bytes, sizeof(DWORD));
    data_size = __builtin_bswap32(data_size);

	// load body
	char * data = (char *) calloc(data_size, sizeof(char));
	bytes_recovered += recover_bytes_lsb4(data, vector, sample_bytes, data_size);

	// load extension
	char extension[MAX_EXT_LEN + 1] = { '\0' };
	int i = 0;
	char ext_c = 0;
	do {
		recover_bytes_lsb4(&ext_c, vector, sample_bytes, sizeof(char));
		extension[i] = ext_c;
		i++;
	} while (ext_c != '\0');
	if (ext) {
		int i = 0;
		char ext_c = 0;
		do {
			recover_bytes(&ext_c, vector, sample_bytes, sizeof(char));
			extension[i] = ext_c;
			i++;
		} while (ext_c != '\0');
	}

	create_file(out_path, extension, data, data_size);

	free(data);
	return bytes_recovered;
}

int recover_bytes_lsb4(char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read) {
	unsigned char * vector_buffer = (unsigned char *) calloc(sample_bytes, BYTE_SIZE);
	unsigned char data_byte = 0;

	unsigned int bytes_read = 0;
	unsigned int bits_read = 0;

	printf("\nbytes_to_read: %d\n", bytes_to_read);
	while (bytes_read < bytes_to_read) {
		fread(vector_buffer, 1, sample_bytes, vector); // TODO: == -1 ?

		data_byte <<= 4;
		data_byte |= lsb4(vector_buffer, sample_bytes);
		bits_read += 4;

		if (bits_read >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
			data[bytes_read] = data_byte;
			data_byte = 0;
			bits_read = 0;
			bytes_read++;
		}
	}
	free(vector_buffer);
	return bytes_to_read;
}

unsigned char lsb4(unsigned char * buffer, int buffer_bytes) {
	return (buffer[buffer_bytes - 1]) & 0x0F;
}
