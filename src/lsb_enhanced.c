#include "../include/lsb_enhanced.h"

static unsigned char lsb(unsigned char * buffer, int buffer_bytes);
static int hideBitEnh(unsigned char *buffer, int size, unsigned char img_bit);

int hideBitEnh(unsigned char *buffer, int size, unsigned char img_bit) {
	if (buffer[size - 1] < 254) {
		return 0;
	}
	int last_idx = size - 1;
	buffer[last_idx] = (buffer[last_idx] & ~0x01) | img_bit;
	return 1;
}

void hide_lsb_enh(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, unsigned int bytes_to_hide) {
	sample_bytes = 1;
	int input_bytes_read = 0;
	int bits_hidden = 0;
	unsigned char bit_to_hide;
	unsigned char * buffer = (unsigned char *) calloc(1, sizeof(char));

	//leo un sample a la vez
	while ((input_bytes_read = fread(buffer, BYTE_SIZE, sample_bytes, orig_file)) > 0) {
		if (bits_hidden >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
			bits_hidden = 0;
			bytes_to_hide--;
			data++;
		}
		if (bytes_to_hide > 0) {
			// agarro el bit mas significativo
			bit_to_hide = ((*data) >> (7 - bits_hidden)) & 0x01;
			// cambio el ultimo bit del buffer de lectura
			if (hideBitEnh(buffer, sample_bytes, bit_to_hide)) {
				bits_hidden++;
			}
		}
		fwrite(buffer, BYTE_SIZE, input_bytes_read, vector); // Writing read data into output file
	}
	if(bytes_to_hide > 0){
		printf("NO TERMINO DE HIDEAR TODO!\n");
		exit(1);
	}
}

int recover_lsb_enh(char * out_path, FILE * vector, unsigned short int sample_bytes, bool ext) {
	// load body size
	DWORD data_size = 0;
	recover_bytes_enh((char *) &data_size, vector, sample_bytes, sizeof(DWORD));
	data_size = __builtin_bswap32(data_size);
	char * data = (char *) calloc(data_size, sizeof(char));
	recover_bytes_enh(data, vector, sample_bytes, data_size);
	char extension[MAX_EXT_LEN + 1] = { '\0' };
	if (ext) {
		int i = 0;
		char ext_c = 0;
		do {
			recover_bytes_enh(&ext_c, vector, sample_bytes, sizeof(char));
			extension[i] = ext_c;
			i++;
		} while (ext_c != '\0');
	}
	// save all to new file
	int bytes_recovered = create_file(out_path, extension, data, data_size);
	free(data);
	return bytes_recovered;
}

int recover_bytes_enh(char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read) {
	sample_bytes = 1;
	unsigned char * vector_buffer = (unsigned char *) calloc(sample_bytes, BYTE_SIZE);
	unsigned char data_byte = 0;

	unsigned int bytes_read = 0;
	unsigned int bits_read = 0;

	while (bytes_read < bytes_to_read) {
		if(fread(vector_buffer, 1, sample_bytes, vector) <= 0){
			printf("Ended file on recover");
			free(vector_buffer);
			return bytes_to_read;
		}

		if (vector_buffer[sample_bytes - 1] >= 254) {
			data_byte <<= 1;
			data_byte |= lsb(vector_buffer, sample_bytes);
			bits_read++;
		}

		if (bits_read == BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
			data[bytes_read] = data_byte;
			data_byte = 0;
			bits_read = 0;
			bytes_read++;
		}
	}
	free(vector_buffer);
	return bytes_to_read;
}

unsigned char lsb(unsigned char * buffer, int buffer_bytes) {
	return (buffer[buffer_bytes - 1]) & 0x01;
}
