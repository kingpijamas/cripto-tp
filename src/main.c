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

static arg parseArg(char * argument);

static void expect_file_to_exist(char * path, error error_code);
static void expect_suffix(char * param, char * suffix, error error_code);

static void fail(error code, char * param);

static void print_help();

int main(int argc, char **argv) {
	if (argc > 1 && streq(argv[1], "-h")) {
		print_help();
		return OK;
	}

	comm command = NO_COMMAND;
	int index;
	char * in_path;	//archivo que se va a ocultar
	char * p_path; 	//archivo que sera el portador
	char * out_path; //archivo de salida
	//algoritmo de esteganografiado, encriptacion, modo y password
	char * steg_type, *enc_type, *mode, *password;
	char * param;
	for (index = 1; index < argc; index++) {
		arg argument = parseArg(argv[index]);
		switch (argument) {
		case embed:
			;
			command = EMBED;
			break;
		case extract:
			;
			command = EXTRACT;
			break;
		case in:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expect_file_to_exist(param, MISSING_IN_FILE);
			in_path = param;
			break;
		case p:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expect_suffix(param, WAV_EXT, INVALID_P_FORMAT);
			expect_file_to_exist(param, MISSING_P_FILE);
			p_path = param;
			break;
		case out:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expect_suffix(param, WAV_EXT, INVALID_OUT_FORMAT);
			out_path = param;
			break;
		case steg:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (!valid_steg_algorithm(param)) {
				fail(INVALID_STEG, param);
			}
			steg_type = param;
			break;
		case a:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (!valid_encryption(param)) {
				fail(INVALID_A, param);
			}
			enc_type = param;
			break;
		case m:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (!valid_mode(param)) {
				fail(INVALID_M, param);
			}
			mode = param;
			break;
		case pass:
			;
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (empty(param)) {
				fail(MISSING_PASS, param);
			}
			password = param;
			break;
		default:
			printf("Comando desconocido: \"%s\". IGNORADO \n", argv[index]);
		}
	}

	if (command == NO_COMMAND) {
		fail(INVALID_OP, NULL);
	}
	//		if(command == STEGOANALYZE){
	//			stuff
	//			return, or fail
	//		}

	//		EMBED OR EXTRACT

	if (command == embed) {
		if (empty(p_path)) {
			fail(INVALID_OP, NULL);
		}
	}
	if (empty(in_path) || empty(out_path) || empty(steg_type)) {
		fail(INVALID_OP, NULL);
	}
	if (empty(enc_type) != empty(mode) || empty(mode) != empty(password)) {
		fail(INVALID_OP, NULL);
	}

	printf("Parameters OK!\n");

	//TODO aca habria que llamar al metodo correspondiente con los
	//parametros recibidos

	switch (command) {
	case EMBED:
		;
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
	case EXTRACT:
		;
		break;
	case ANALYZE:
		;
		// TODO
		break;
	default:
		printf("No command assigned!");
		exit(1);
		break;
	}

	return OK;
}

arg parseArg(char * argument) {
	if (streq(argument, "-embed"))
		return embed;
	if (streq(argument, "-extract"))
		return extract;
	if (streq(argument, "-in"))
		return in;
	if (streq(argument, "-p"))
		return p;
	if (streq(argument, "-out"))
		return out;
	if (streq(argument, "-steg"))
		return steg;
	if (streq(argument, "-a"))
		return a;
	if (streq(argument, "-m"))
		return m;
	if (streq(argument, "-pass"))
		return pass;
	return unknown;
}

void expect_file_to_exist(char * path, error error_code) {
	if (!file_exists(path)) {
		fail(error_code, NULL);
	}
}

void expect_suffix(char * param, char * suffix, error error_code) {
	if (!has_suffix(param, suffix)) {
		fail(error_code, param);
	}
}

int file_exists(char * path) {
	return access(path, F_OK) != -1;
}

int streq(char * str1, char * str2) {
	return strcmp(str1, str2) == 0;
}

int empty(char * str) {
	return str == 0 || *str == '\0';
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
	return strcmp(algorithm, "LSB1") == 0 || strcmp(algorithm, "LSB4") == 0 || strcmp(algorithm, "LSBE") == 0;
}

int valid_encryption(char * encryption) {
	return strcmp(encryption, "aes128") == 0 || strcmp(encryption, "aes192") == 0 || strcmp(encryption, "aes256") == 0
			|| strcmp(encryption, "des") == 0;
}

int valid_mode(char * mode) {
	return strcmp(mode, "ecb") == 0 || strcmp(mode, "cfb") == 0 || strcmp(mode, "ofb") == 0 || strcmp(mode, "cbc") == 0;
}

void print_help() {
	printf("----------------------------------------------------------------------------------------------\n");
	printf("Help Menu \n");
	printf("----------------------------------------------------------------------------------------------\n");
	printf("-embed \t\t\t\t\t\t Indica que se va a ocultar información \n");
	printf("-extract \t\t\t\t\t Indica que se va a extraer información \n");
	printf("-in [file] \t\t\t\t\t Archivo que se va a ocultar \n");
	printf("-p [wavefile] \t\t\t\t\t Archivo wav portador \n");
	printf("-out [file] \t\t\t\t\t Archivo de salida obtenido \n");
	printf("-steg <LSB1 | LSB4 | LSBE> \t\t\t Algoritmo de esteganografiado \n");
	printf("-a <aes128 | aes192 | aes256 | des> (OPTIONAL) \n");
	printf("-m <ecb | cfb | ofb | cbc> (OPTIONAL) \n");
	printf("-pass [password] (OPTIONAL) \t\t\t password de encripcion \n");
}

void fail(error err, char * param) {
	switch (err) {
	case INC_PARAMC:
		printf("Numero incorrecto de parametros\n");
		break;
	case INVALID_OP:
		printf("Operacion invalida\n");
		break;
	case INVALID_A:
		printf("No es valida la encriptacion\n");
		break;
	case INVALID_M:
		printf("No es valido el modo\n");
		break;
	case MISSING_P:
		printf("Falta el parametro -p\n");
		break;
	case MISSING_OUT_FILE:
		printf("Falta el parametro -out\n");
		break;
	case MISSING_STEG:
		printf("Falta el parametro -steg\n");
		break;
	case MISSING_PASS:
		printf("Falta la password\n");
		break;
	case MISSING_IN_FILE:
	case MISSING_P_FILE:
		printf("No existe el archivo %s\n", param);
		break;
	case INVALID_P_FORMAT:
		printf("Formato incorrecto. El parametro -p espera .wav \n");
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
