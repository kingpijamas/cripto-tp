#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/define.h"
#include "../include/WavHeaderUtils.h"

static int file_exists(char * path);
static int streq(char * str1, char * str2);
static int empty(char * str);
static int valid_steg_algorithm(char * algorithm);
static int valid_encryption(char * encryption);
static int valid_mode(char * mode);
static int has_suffix(char * str, char * suffix);

static void expect_param_to_eq(char * param, char * expected, int error_code);
static void expect_file_to_exist(char * path, int error_code);
static void expect_suffix(char * param, char * suffix, int error_code);
static void expect_in_or_p(char * param, int command);

static void fail(int code, char * param);

static void print_help();
static void parse_command(char * param, int * command);

int main(int argc, char **argv) {
	if (argc > 1 && streq(argv[1], "-h")) {
		print_help();
		return OK;
	}

	// printf("argc: %d\n", argc);
	// int i;
	// for(i = 0; i < argc; i++){
	//     printf("argc %d : %s \n", i, argv[i]);
	// }

	if (argc < 8 || argc > 16) {
		fail(INC_PARAMC, NULL);
	}

	int command = NO_COMMAND;
	int index;
	char * in_path;	//archivo que se va a ocultar
	char * p_path; 	//archivo que sera el portador
	char * out_path; //archivo de salida
	//algoritmo de esteganografiado, encriptacion, modo y password
	char * steg_type, *enc_type, *mode, *pass;

	parse_command(argv[1], &command);
	switch (command) {
	case EMBED:
		expect_param_to_eq(argv[2], "-in", MISSING_IN);
		break;
	case EXTRACT:
		expect_param_to_eq(argv[2], "-p", MISSING_P);
		break;
	}
	expect_in_or_p(argv[2], command);

	for (index = 3; index < argc; index++) {
		char * param = argv[index];

		switch (index) {
		case 3:
			switch (command) {
			case EMBED:
				//TODO chequear esto porque no necesariamente es .bmp
				expect_suffix(param, BMP_EXT, INVALID_IN_FORMAT);
				expect_file_to_exist(param, MISSING_IN_FILE);
				in_path = param;
				break;
			case EXTRACT:
				expect_suffix(param, WAV_EXT, INVALID_P_FORMAT);
				expect_file_to_exist(param, MISSING_P_FILE);
				p_path = param;
				break;
			}
			break;
		case 4:
			switch (command) {
			case EMBED:
				expect_param_to_eq(param, "-p", MISSING_P);
				break;
			case EXTRACT:
				expect_param_to_eq(param, "-out", MISSING_OUT);
				break;
			}
			break;
		case 5:
			switch (command) {
			case EMBED:
				expect_suffix(param, WAV_EXT, INVALID_P_FORMAT);
				expect_file_to_exist(param, MISSING_P_FILE);
				p_path = param;
				break;
			case EXTRACT:
				expect_file_to_exist(param, MISSING_OUT_FILE);
				out_path = param;
				break;
			}
			break;
		case 6:
			switch (command) {
			case EMBED:
				expect_param_to_eq(param, "-out", MISSING_OUT);
				break;
			case EXTRACT:
				expect_param_to_eq(param, "-steg", MISSING_STEG);
				break;
			}
			break;
		case 7:
			switch (command) {
			case EMBED:
				expect_suffix(param, WAV_EXT, INVALID_OUT_FORMAT);
				expect_file_to_exist(param, MISSING_OUT_FILE);
				out_path = param;
				break;
			case EXTRACT:
				if (!valid_steg_algorithm(param)) { // XXX
					fail(INVALID_STEG, param);
				}
				steg_type = param;
				break;
			}
			break;
			// case 8:
			//     if(embed == 1){
			//         if(strcmp(param, "-steg") != 0){
			//             printf("Falta el parametro -steg\n");
			//             error = MISSING_STEG;
			//         }
			//     }
			//     break;
			// case 9:
			//     if(embed == 1){
			//         if(strcmp(param, "LSB1") == 0
			//             || strcmp(param, "LSB4") == 0
			//             || strcmp(param, "LSBE") == 0){
			//             steg_type = param;
			//         }else{
			//             printf("No es valido el algoritmo de esteganografiado \n");
			//             error = INVALID_STEG;
			//         }
			//     }
			//     break;
		default:
			if (command == EMBED && index < 10) {
				if (index == 8) {
					expect_param_to_eq(param, "-steg", MISSING_STEG);
					break;
				}
				if (index == 9) {
					if (!valid_steg_algorithm(param)) {
						fail(INVALID_STEG, param);
					}
					steg_type = param;
					break;
				}
			}
			if (streq(param, "-a")) {
				index++; // XXX
				param = argv[index];
				if (!valid_encryption(param)) {
					fail(INVALID_A, param);
				}
				enc_type = param;
			} else if (streq(param, "-m")) {
				index++; // XXX
				param = argv[index];
				if (!valid_mode(param)) {
					fail(INVALID_M, param);
				}
				mode = param;
			} else if (streq(param, "-pass")) {
				index++; // XXX
				param = argv[index];
				if (empty(param)) {
					fail(MISSING_PASS, param);
				}
				pass = param;
			}
			break;
		}
	}

	if (command == EMBED && argc > 10) {
		if (empty(pass)) {
			fail(MISSING_PASS, argv[index]);
		}

		if (empty(mode)) {
			mode = DEFAULT_MODE;
		}

		if (empty(enc_type)) {
			enc_type = DEFAULT_ENCTYPE;
		}
	}

	//TODO aca habria que llamar al metodo correspondiente con los
	//parametros recibidos

	switch (command) {
	case EMBED: ;
		// TODO
		break;
	case EXTRACT: ;
		FILE * vector = fopen(p_path, "rb");
		FILE * outfile = fopen(out_path, "wb");
		FILE * img = fopen(in_path, "rb");
		struct WAV_HEADER header = parseHeader(vector);
		fwrite(&header, 1, sizeof(header), outfile);
		fseek(img, 0L, SEEK_END);
		DWORD sz = ftell(img);
		sz += sizeof(DWORD);
		rewind(img);
		unsigned short int vector_size = header.bits_per_sample / 8;

		unsigned char *bufferHide = (unsigned char *) malloc(sz);
		memcpy(bufferHide, img, sz + 1);
		fread(bufferHide, sz, 1, img);

		if (strcmp(steg_type, "LSB1") == 0) {
			hideLSB(vector, outfile, bufferHide, sz, vector_size);
		} else if (strcmp(steg_type, "LSB4") == 0) {
			hideLSB4(vector, outfile, bufferHide, sz, vector_size);
		} else if (strcmp(steg_type, "LSBE") == 0) {
			hideLSBEnh(vector, outfile, bufferHide, sz, vector_size);
		} else {
			break;
		}
		return OK;
	case ANALYZE: ;
		// TODO
		break;
	default:
		printf("No command assigned!");
		exit(1);
		break;
	}

	return OK;
}

void expect_param_to_eq(char * param, char * expected, int error_code) {
	if (!streq(param, expected)) {
		fail(error_code, param);
	}
}

void expect_file_to_exist(char * path, int error_code) {
	if (!file_exists(path)) {
		fail(error_code, NULL);
	}
}

void expect_suffix(char * param, char * suffix, int error_code) {
	if (!has_suffix(param, suffix)) {
		fail(error_code, param);
	}
}

void expect_in_or_p(char * param, int command) {
	switch (command) {
	case EMBED:
		expect_param_to_eq(param, "-in", MISSING_IN);
		break;
	case EXTRACT:
		expect_param_to_eq(param, "-p", MISSING_P);
		break;
	}
}

int file_exists(char * path) {
	return access(path, F_OK) != -1;
}

int streq(char * str1, char * str2) {
	return strcmp(str1, str2) == 0;
}

int empty(char * str) {
	return *str == '\0';
}

int has_suffix(char * str, char * suffix) {
	char * last_appearance = strrchr(str, suffix);
	if (last_appearance == NULL || strlen(last_appearance) > strlen(suffix)) {
		// 'suffix' is not actually a suffix, just a substring
		return false;
	}
	return last_appearance;
}

int valid_steg_algorithm(char * algorithm) {
	return strcmp(algorithm, "LSB1") == 0 || strcmp(algorithm, "LSB4") == 0
			|| strcmp(algorithm, "LSBE") == 0;
}

int valid_encryption(char * encryption) {
	return strcmp(encryption, "aes128") == 0
			|| strcmp(encryption, "aes192") == 0
			|| strcmp(encryption, "aes256") == 0
			|| strcmp(encryption, "des") == 0;
}

int valid_mode(char * mode) {
	return strcmp(mode, "ecb") == 0 || strcmp(mode, "cfb") == 0
			|| strcmp(mode, "ofb") == 0 || strcmp(mode, "cbc") == 0;
}

void parse_command(char * param, int * command) {
	if (streq(param, "-embed")) {
		*command = EMBED;
	} else if (streq(param, "-extract")) {
		*command = EXTRACT;
	} else {
		fail(INVALID_OP, param);
	}
}

void print_help() {
	printf(
			"----------------------------------------------------------------------------------------------\n");
	printf("Help Menu \n");
	printf(
			"----------------------------------------------------------------------------------------------\n");
	printf("-embed \t\t\t\t\t\t Indica que se va a ocultar información \n");
	printf("-extract \t\t\t\t\t Indica que se va a extraer información \n");
	printf("-in [file] \t\t\t\t\t Archivo que se va a ocultar \n");
	printf("-p [wavefile] \t\t\t\t\t Archivo wav portador \n");
	printf("-out [file] \t\t\t\t\t Archivo de salida obtenido \n");
	printf(
			"-steg <LSB1 | LSB4 | LSBE> \t\t\t Algoritmo de esteganografiado \n");
	printf("-a <aes128 | aes192 | aes256 | des> (OPTIONAL) \n");
	printf("-m <ecb | cfb | ofb | cbc> (OPTIONAL) \n");
	printf("-pass [password] (OPTIONAL) \t\t\t password de encripcion \n");
}

void fail(int code, char * param) {
	switch (code) {
	case INC_PARAMC:
		printf("Numero incorrecto de parametros\n");
		break;
	case INVALID_A:
		printf("No es valida la encriptacion\n");
		break;
	case INVALID_M:
		printf("No es valido el modo\n");
		break;
	case INVALID_OP:
		printf("Operacion invalida\n");
		break;
	case MISSING_IN:
		printf("Falta el parametro -in\n");
		break;
	case MISSING_P:
		printf("Falta el parametro -p\n");
		break;
	case MISSING_OUT:
		printf("Falta el parametro -out\n");
		break;
	case MISSING_STEG:
		printf("Falta el parametro -steg\n");
		break;
	case MISSING_PASS:
		printf("Falta la password\n");
		break;
	case MISSING_IN_FILE:
	case MISSING_OUT_FILE:
	case MISSING_P_FILE:
		printf("No existe el archivo %s\n", param);
		break;
	case INVALID_P_FORMAT:
		printf("Formato incorrecto. El parametro -p espera .wav \n");
		break;
	case INVALID_IN_FORMAT:
		printf("Formato incorrecto. El parametro -in espera .bmp \n");
		break;
	case INVALID_OUT_FORMAT:
		printf("Formato incorrecto. El parametro -p espera .wav \n");
		break;
	case INVALID_STEG:
		printf("No es valido el algoritmo de esteganografiado \n");
		break;
	}
	exit(-1);
}
