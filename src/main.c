#include "../include/main.h"

int main(int argc, char **argv) {
	printf("Antes del help");
	if (argc == 1 || (streq(argv[1], "-h") || streq(argv[1], "--help"))) {
		print_help();
		return SYS_OK;
	}
	printf("dps del help");

	comm command = NO_COMMAND;
	int index;
	char * in_path;	//archivo que se va a ocultar
	char * p_path; 	//archivo que sera el portador
	char * out_path; //archivo de salida
	//algoritmo de esteganografiado, encriptacion, modo y password
	char * steg_type, *password;
	enc_type enc_type = UNKNOWN_ENC_TYPE;
	enc_mode enc_mode = UNKNOWN_ENC_MODE;
	char * param;
	for (index = 1; index < argc; index++) {
		arg argument = parseArg(argv[index]);
		switch (argument) {
		when(EMBED_ARG)
			printf("Embed");
			command = EMBED;
			break;
		when(EXTRACT_ARG)
			printf("Extract");
			command = EXTRACT;
			break;
		when(IN_ARG)
			printf("In");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expectFileToExist(param, MISSING_IN_FILE);
			in_path = param;
			break;
		when(P_ARG)
			printf("P arg");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expectSuffix(param, WAV_EXT, INVALID_P_FORMAT);
			expectFileToExist(param, MISSING_P_FILE);
			p_path = param;
			break;
		when(OUT_ARG)
			printf("Out arg");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (command == EMBED) {
				expectSuffix(param, WAV_EXT, INVALID_OUT_FORMAT);
			}
			out_path = param;
			break;
		when(STEG_ARG)
			printf("Steg arg");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (!validStegAlgorithm(param)) {
				fail(INVALID_STEG, param);
			}
			steg_type = param;
			break;
		when(A_ARG)
			printf("A arg");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			enc_type = parse_enc_type(argv[index]);
			break;
		when(M_ARG)
			printf("M arg");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			enc_mode = parse_enc_mode(argv[index]);
			break;
		when(PASS_ARG)
			printf("Pass Arg");
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

	printf("Command es %d ", command);
	if (command == NO_COMMAND) {
		fail(INVALID_OP, NULL);
	}
	printf("Parameters OK!\n");

	FILE * vector, *outfile, *payload;
	WAV_HEADER header;
	DWORD payload_size;

	printf("Antes del switch\n");

	switch (command) {
	when(EMBED)
		printf("Embed entered");
		if (empty(in_path)) {
			fail(INVALID_OP, NULL);
		}
		if (empty(p_path) || empty(out_path) || empty(steg_type)) {
			fail(INVALID_OP, NULL);
		}
		if ((enc_type == UNKNOWN_ENC_TYPE) != (enc_mode == UNKNOWN_ENC_MODE)
				|| (enc_mode == UNKNOWN_ENC_MODE) != empty(password)) {
			fail(INVALID_OP, NULL);
		}
		vector = fopen(p_path, "rb");
		outfile = fopen(out_path, "wb");
		payload = fopen(in_path, "rb");
		header = parseHeader(vector);
		fwrite(&header, 1, sizeof(header), outfile);

		// Gets payload's size
		fseek(payload, 0L, SEEK_END);
		payload_size = ftell(payload);
		payload_size += sizeof(DWORD);
		rewind(payload);

		unsigned short int vector_size = header.bits_per_sample / 8;
		unsigned char * buffer_hide = (unsigned char *) malloc(payload_size);
		// memcpy(buffer_hide, payload, payload_size + 1);
		fread(buffer_hide, payload_size, 1, payload);

		if (streq(steg_type, "LSB1")) {
			hideLSB1(vector, outfile, buffer_hide, payload_size, vector_size);
		} else if (streq(steg_type, "LSB4")) {
			hideLSB4(vector, outfile, buffer_hide, payload_size, vector_size);
		} else if (streq(steg_type, "LSBE")) {
			hideLSBEnh(vector, outfile, buffer_hide, payload_size, vector_size);
		}
		return SYS_OK;
	when(EXTRACT)
		printf("Valida");
		if (empty(p_path) || empty(out_path) || empty(steg_type)) {
			fail(INVALID_OP, NULL);
		}
		if ((enc_type == UNKNOWN_ENC_TYPE) != (enc_mode == UNKNOWN_ENC_MODE)
				|| (enc_mode == UNKNOWN_ENC_MODE) != empty(password)) {
			fail(INVALID_OP, NULL);
		}
		printf("Empiezo");
		vector = fopen(p_path, "rb");
		outfile = fopen(out_path, "wb");
		header = parseHeader(vector);
		printf("Ya parseó");
		int bytes_per_sample = header.bits_per_sample / 8;
		if (streq(steg_type, "LSB1")) {
			recoverLSB1(vector, outfile, bytes_per_sample);
		} else if (streq(steg_type, "LSB4")) {
			recoverLSB4(vector, outfile, bytes_per_sample);
		} else if (streq(steg_type, "LSBE")) {
			recoverLSBEnh(vector, outfile, bytes_per_sample);
		}
		return SYS_OK;
	when(ANALYZE)
		// TODO
		break;
	default:
		printf("No command assigned!");
		exit(1);
		break;
	}
	return SYS_OK;
}

enc_type parse_enc_type(char * type) {
	if (streq(type, "aes128")) {
		return AES128;
	}
	if (streq(type, "aes192")) {
		return AES192;
	}
	if (streq(type, "aes256")) {
		return AES256;
	}
	if (streq(type, "des")) {
		return DES;
	}
	fail(INVALID_A, type);
	exit(-1);
}

enc_mode parse_enc_mode(char * mode) {
	if (streq(mode, "ecb")) {
		return ECB;
	}
	if (streq(mode, "cfb")) {
		return CFB;
	}
	if (streq(mode, "ofb")) {
		return OFB;
	}
	if (streq(mode, "cbc")) {
		return CBC;
	}
	fail(INVALID_M, mode);
	exit(-1);
}

arg parseArg(char * argument) {
	if (streq(argument, "-embed")) {
		return EMBED_ARG;
	}
	if (streq(argument, "-extract")) {
		return EXTRACT_ARG;
	}
	if (streq(argument, "-in")) {
		return IN_ARG;
	}
	if (streq(argument, "-p")) {
		return P_ARG;
	}
	if (streq(argument, "-out")) {
		return OUT_ARG;
	}
	if (streq(argument, "-steg")) {
		return STEG_ARG;
	}
	if (streq(argument, "-a")) {
		return A_ARG;
	}
	if (streq(argument, "-m")) {
		return M_ARG;
	}
	if (streq(argument, "-pass")) {
		return PASS_ARG;
	}
	return UNKNOWN_ARG;
}

void expectFileToExist(char * path, error error_code) {
	if (!fileExists(path)) {
		fail(error_code, NULL);
	}
}

void expectSuffix(char * param, char * suffix, error error_code) {
	if (!hasSuffix(param, suffix)) {
		fail(error_code, param);
	}
}

int fileExists(char * path) {
	return access(path, F_OK) != -1;
}

int streq(char * str1, char * str2) {
	return strcmp(str1, str2) == 0;
}

int empty(char * str) {
	return str == 0 || *str == '\0';
}

int hasSuffix(char * str, char * suffix) {
	int str_len = strlen(str);
	int suff_len = strlen(suffix);

	if (suff_len > str_len) {
		return false;
	}

	int suff_start = str_len - suff_len;
	return streq(str + suff_start, suffix);
}

int validStegAlgorithm(char * algorithm) {
	return strcmp(algorithm, "LSB1") == 0 || strcmp(algorithm, "LSB4") == 0 || strcmp(algorithm, "LSBE") == 0;
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
