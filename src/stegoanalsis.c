#include "../include/stegoanalisis.h"

static void testLSB1(FILE * vector, WAV_HEADER header);
static void testLSB4(FILE * vector, WAV_HEADER header);
static void testLSBEnh(FILE * vector, WAV_HEADER header);

void stegoanalyze(char * p_path) {
	printf("STEGOANALYZING . . .\n");
	FILE * vector = fopen(p_path, "rb");
	WAV_HEADER header = parse_header(vector);
	printf("\n");
	testLSB1(vector, header);
	fclose(vector);
	vector = fopen(p_path, "rb");
	header = parse_header(vector);
	printf("\n");
	testLSB4(vector, header);
	fclose(vector);
	vector = fopen(p_path, "rb");
	header = parse_header(vector);
	printf("\n");
	testLSBEnh(vector, header);
	fclose(vector);
}

void testLSB1(FILE * vector, WAV_HEADER header) {
	DWORD data_size = 0;
	unsigned int bytes_per_sample = header.bits_per_sample / BITS_PER_BYTE;
	int bytes_recovered = recover_bytes_lsb1((char *) &data_size, vector, bytes_per_sample, sizeof(DWORD));
	data_size = __builtin_bswap32(data_size);
	printf("Data size: %u\n", data_size);
	printf("Needed size: %u\nFile size: %u\n", data_size * 8 * bytes_per_sample, header.overall_size);
	if (data_size * 8 * bytes_per_sample > header.overall_size) {
		printf("Is not LSB1\n\tREASON: %s\n", "File size and payload size mismatch");
		return;
	}
	char * data = (char *) calloc(data_size, sizeof(char));
	bytes_recovered += recover_bytes_lsb1(data, vector, bytes_per_sample, data_size);
	char extension[MAX_EXT_LEN + 1] = { '\0' };
	int i = 0;
	char ext_c = 0;
	do {
		if (recover_bytes_lsb1(&ext_c, vector, bytes_per_sample, sizeof(char)) == 0) {
			printf("Is not LSB1\n\tREASON: %s\n", "Met end of file without '\0' extension ending");
			return;
		}
		if (i == 0) {
			if (ext_c != '.' && ext_c != '\0') {
				printf("Is not LSB1\n\tREASON: %s\n", "First extension character should be '.' or empty");
				printf("\tDetected: '%c'\n", ext_c);
				return;
			}
		} else {
			if (!((ext_c > 'a' && ext_c < 'z') || (ext_c > 'A' && ext_c < 'Z') || ext_c == '\0')) {
				printf("Is not LSB1\n\tREASON: %s\n", "Extension characters should meet '.[a-zA-Z]*' regex");
				printf("\tDetected: '%c'\n", ext_c);
				return;
			}
		}
		extension[i] = ext_c;
		i++;
	} while (ext_c != '\0');
	printf("\n### File extension matches LSB1 format ###\nIts extension is '%s'\n", extension);
	create_file("/tmp/stego", "", data, data_size);
	char * command = (char *) calloc(120, sizeof(char));
	printf("File type detection:\n");
	strcpy(command, "file /tmp/stego | cut -d' ' -f2- | sed 's/^data/Undefined/g' && rm /tmp/stego");
	system(command);
}

void testLSB4(FILE * vector, WAV_HEADER header) {
	DWORD data_size = 0;
	unsigned int bytes_per_sample = header.bits_per_sample / BITS_PER_BYTE;
	int bytes_recovered = recover_bytes_lsb4((char *) &data_size, vector, bytes_per_sample, sizeof(DWORD));
	data_size = __builtin_bswap32(data_size);
	printf("Data size: %u\n", data_size);
	if (data_size * 2 * bytes_per_sample > header.overall_size) {
		printf("Is not LSB4\n\tREASON: %s\n", "File size and payload size mismatch");
		printf("Needed size: %u\nFile size: %u\n", data_size * 2 * bytes_per_sample, header.overall_size);
		return;
	}
	char * data = (char *) calloc(data_size, sizeof(char));
	bytes_recovered += recover_bytes_lsb4(data, vector, bytes_per_sample, data_size);
	char extension[MAX_EXT_LEN + 1] = { '\0' };
	int i = 0;
	char ext_c = 0;
	do {
		if (recover_bytes_lsb4(&ext_c, vector, bytes_per_sample, sizeof(char)) == 0) {
			printf("Is not LSB4\n\tREASON: %s\n", "Met end of file without '\0' extension ending");
			return;
		}
		if (i == 0) {
			if (ext_c != '.' && ext_c != '\0') {
				printf("Is not LSB4\n\tREASON: %s\n", "First extension character should be '.' or empty");
				printf("\tDetected: '%c'\n", ext_c);
				return;
			}
		} else {
			if (!((ext_c > 'a' && ext_c < 'z') || (ext_c > 'A' && ext_c < 'Z') || ext_c == '\0')) {
				printf("Is not LSB4\n\tREASON: %s\n", "Extension characters should meet '.[a-zA-Z]*' regex");
				printf("\tDetected: '%c'\n", ext_c);
				return;
			}
		}
		extension[i] = ext_c;
		i++;
	} while (ext_c != '\0');
	printf("\n### File extension matches LSB4 format ###\nIts extension is '%s'\n", extension);
	create_file("/tmp/stego", "", data, data_size);
	char * command = (char *) calloc(120, sizeof(char));
	printf("File type detection:\n");
	strcpy(command, "file /tmp/stego | cut -d' ' -f2- | sed 's/^data/Undefined/g' && rm /tmp/stego");
	system(command);
}

void testLSBEnh(FILE * vector, WAV_HEADER header) {
	DWORD data_size = 0;
	unsigned int bytes_per_sample = header.bits_per_sample / BITS_PER_BYTE;
	int bytes_recovered = recover_bytes_enh((char *) &data_size, vector, bytes_per_sample, sizeof(DWORD));
	data_size = __builtin_bswap32(data_size);
	printf("Data size: %u\n", data_size);
	if (data_size * 8 > header.overall_size) {
		printf("Is not LSBEnh\n\tREASON: %s\n", "File size and payload size mismatch");
		printf("Needed (at least) size: %u\nFile size: %u\n", data_size * 8, header.overall_size);
		return;
	}
	char * data = (char *) calloc(data_size, sizeof(char));
	bytes_recovered += recover_bytes_enh(data, vector, bytes_per_sample, data_size);
	char extension[MAX_EXT_LEN + 1] = { '\0' };
	int i = 0;
	char ext_c = 0;
	do {
		if (recover_bytes_enh(&ext_c, vector, bytes_per_sample, sizeof(char)) == 0) {
			printf("Is not LSBEnh\n\tREASON: %s\n", "Met end of file without '\0' extension ending");
			return;
		}
		if (i == 0) {
			if (ext_c != '.' && ext_c != '\0') {
				printf("Is not LSBEnh\n\tREASON: %s\n", "First extension character should be '.' or empty");
				printf("\tDetected: '%c'\n", ext_c);
				return;
			}
		} else {
			if (!((ext_c > 'a' && ext_c < 'z') || (ext_c > 'A' && ext_c < 'Z') || ext_c == '\0')) {
				printf("Is not LSBEnh\n\tREASON: %s\n", "Extension characters should meet '.[a-zA-Z]*' regex");
				printf("\tDetected: '%c'\n", ext_c);
				return;
			}
		}
		extension[i] = ext_c;
		i++;
	} while (ext_c != '\0');
	printf("\n### File extension matches LSB Enhanced format ###\nIts extension is '%s'\n", extension);
	create_file("/tmp/stego", "", data, data_size);
	char * command = (char *) calloc(120, sizeof(char));
	printf("File type detection:\n");
	strcpy(command, "file /tmp/stego | cut -d' ' -f2- | sed 's/^data/Undefined/g' && rm /tmp/stego");
	system(command);
}
