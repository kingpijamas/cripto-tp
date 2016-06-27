#include "../include/main.h"

static int file_exists(char * path);
static int streq(char * str1, char * str2);
static int empty(char * str);
static int valid_steg_algorithm(char * algorithm);
static ENC_TYPE parse_enc_type(char * type);
static ENC_MODE parse_enc_mode(char * mode);
static int has_suffix(char * str, char * suffix);
static arg parse_arg(char * argument);
static void expect_file_to_exist(char * path, error error_code);
static void expect_suffix(char * param, char * suffix, error error_code);
static void fail(error error_code, char * param);
static void print_help();
static void extract(char * p_path, char * out_path, char * steg_type, char * password, ENC_TYPE enc_type,
		ENC_MODE enc_mode);
static void embed(char * in_path, char * p_path, char * out_path, char * steg_type, char * password, ENC_TYPE enc_type,
		ENC_MODE enc_mode);

int main(int argc, char **argv) {
	printf("Antes del help\n");
	if (argc == 1 || (streq(argv[1], "-h") || streq(argv[1], "--help"))) {
		print_help();
		return SYS_OK;
	}
	printf("dps del help\n");

	comm command = NO_COMMAND;
	char * in_path = NULL;	//archivo que se va a ocultar
	char * p_path = NULL; 	//archivo que sera el portador
	char * out_path = NULL; //archivo de salida
	//algoritmo de esteganografiado, encriptacion, modo y password
	char * steg_type = NULL;
	char * password = NULL;
	ENC_TYPE enc_type = UNKNOWN_ENC_TYPE;
	ENC_MODE enc_mode = UNKNOWN_ENC_MODE;
	char * param = NULL;
	for (int index = 1; index < argc; index++) {
		arg argument = parse_arg(argv[index]);
		switch (argument) {
		when(EMBED_ARG)
			printf("Embed\n");
			command = EMBED;
			break;
		when(EXTRACT_ARG)
			printf("Extract\n");
			command = EXTRACT;
			break;
		when(IN_ARG)
			printf("In\n");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expect_file_to_exist(param, MISSING_IN_FILE);
			in_path = param;
			break;
		when(P_ARG)
			printf("P arg\n");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			expect_suffix(param, WAV_EXT, INVALID_P_FORMAT);
			expect_file_to_exist(param, MISSING_P_FILE);
			p_path = param;
			break;
		when(OUT_ARG)
			printf("Out arg\n");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (command == EMBED) {
				expect_suffix(param, WAV_EXT, INVALID_OUT_FORMAT);
			}
			out_path = param;
			break;
		when(STEG_ARG)
			printf("Steg arg\n");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			param = argv[index];
			if (!valid_steg_algorithm(param)) {
				fail(INVALID_STEG, param);
			}
			steg_type = param;
			break;
		when(A_ARG)
			printf("A arg\n");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			enc_type = parse_enc_type(argv[index]);
			break;
		when(M_ARG)
			printf("M arg\n");
			if (++index >= argc) {
				fail(INC_PARAMC, NULL);
			}
			enc_mode = parse_enc_mode(argv[index]);
			break;
		when(PASS_ARG)
			printf("Pass Arg\n");
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
			printf("Comando desconocido: \"%s\". IGNORADO\n", argv[index]);
		}
	}

	printf("Command es %d ", command);
	if (command == NO_COMMAND) {
		fail(INVALID_OP, NULL);
	}
	printf("Parameters OK!\n");

	switch (command) {
	when(EMBED)
		printf("Embed entered\n");
		embed(in_path, p_path, out_path, steg_type, password, enc_type, enc_mode);
		return SYS_OK;
	when(EXTRACT)
		extract(p_path, out_path, steg_type, password, enc_type, enc_mode);
		return SYS_OK;
	when(ANALYZE)
		stegoanalyze(p_path);
		break;
	default:
		printf("No command assigned!\n");
		exit(1);
		break;
	}
	return SYS_OK;
}

void embed(char * in_path, char * p_path, char * out_path, char * steg_type, char * password, ENC_TYPE enc_type,
		ENC_MODE enc_mode) {
	if (empty(in_path)) {
		fail(INVALID_OP, NULL);
	}
	if (empty(p_path) || empty(out_path) || empty(steg_type)) {
		fail(INVALID_OP, NULL);
	}
	bool encrypt = !empty(password);
	if (encrypt && (enc_type == UNKNOWN_ENC_TYPE || enc_mode == UNKNOWN_ENC_MODE)) {
		printf("%d %d '%s'\n", enc_type, enc_mode, password);
		fail(INVALID_OP, NULL);
	}

	char * data = (char *) calloc(1, sizeof(char *));
  int marshalled_size = marshall_plain(in_path, &data);

	if (encrypt) {
	 	in_path = encrypt_buffer(data, enc_type, enc_mode, password);
		marshalled_size = marshall_encrypted(in_path, &data);
	}

	FILE * vector = fopen(p_path, "rb");
	FILE * out_file = fopen(out_path, "wb");

	WAV_HEADER header = parse_header(vector);
	fwrite(&header, 1, sizeof(header), out_file);
	unsigned short int bytes_per_sample = header.bits_per_sample / BITS_PER_BYTE;

	if (streq(steg_type, "LSB1")) {
		hide_lsb1(out_file, vector, bytes_per_sample, data, marshalled_size);
	} else if (streq(steg_type, "LSB4")) {
		hide_lsb4(out_file, vector, bytes_per_sample, data, marshalled_size);
	} else if (streq(steg_type, "LSBE")) {
		hide_lsb_enh(out_file, vector, bytes_per_sample, data, marshalled_size);
	}

	free(data);
	fclose(vector);
	fclose(out_file);
}

void extract(char * p_path, char * out_path, char * steg_type, char * password, ENC_TYPE enc_type, ENC_MODE enc_mode) {
	if (empty(p_path) || empty(out_path) || empty(steg_type)) {
		fail(INVALID_OP, NULL);
	}
	if ((enc_type == UNKNOWN_ENC_TYPE) != (enc_mode == UNKNOWN_ENC_MODE)
			|| (enc_mode == UNKNOWN_ENC_MODE) != empty(password)) {
		fail(INVALID_OP, NULL);
	}

	FILE * vector = fopen(p_path, "rb");
	WAV_HEADER header = parse_header(vector);
	int bytes_per_sample = header.bits_per_sample / BITS_PER_BYTE;

	if (streq(steg_type, "LSB1")) {
		recover_lsb1(out_path, vector, bytes_per_sample); //TODO
	} else if (streq(steg_type, "LSB4")) {
		recover_lsb4(out_path, vector, bytes_per_sample); //TODO
	} else if (streq(steg_type, "LSBE")) {
		recover_lsb_enh(out_path, vector, bytes_per_sample); //TODO
	}

	fclose(vector);
}

ENC_TYPE parse_enc_type(char * type) {
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

ENC_MODE parse_enc_mode(char * mode) {
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

arg parse_arg(char * argument) {
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
	int str_len = strlen(str);
	int suff_len = strlen(suffix);

	if (suff_len > str_len) {
		return false;
	}

	int suff_start = str_len - suff_len;
	return streq(str + suff_start, suffix);
}

int valid_steg_algorithm(char * algorithm) {
	return strcmp(algorithm, "LSB1") == 0 || strcmp(algorithm, "LSB4") == 0 || strcmp(algorithm, "LSBE") == 0;
}

void print_help() {
	printf("----------------------------------------------------------------------------------------------\n");
	printf("Help Menu\n");
	printf("----------------------------------------------------------------------------------------------\n");
	printf("-embed \t\t\t\t\t\t Indica que se va a ocultar información\n");
	printf("-extract \t\t\t\t\t Indica que se va a extraer información\n");
	printf("-in [file] \t\t\t\t\t Archivo que se va a ocultar\n");
	printf("-p [wavefile] \t\t\t\t\t Archivo wav portador\n");
	printf("-out [file] \t\t\t\t\t Archivo de salida obtenido\n");
	printf("-steg <LSB1 | LSB4 | LSBE> \t\t\t Algoritmo de esteganografiado\n");
	printf("-a <aes128 | aes192 | aes256 | des> (OPTIONAL)\n");
	printf("-m <ecb | cfb | ofb | cbc> (OPTIONAL)\n");
	printf("-pass [password] (OPTIONAL) \t\t\t password de encripcion\n");
}

void fail(error error_code, char * param) {
	switch (error_code) {
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
		printf("Formato incorrecto. El parametro -p espera .wav\n");
		break;
	case INVALID_OUT_FORMAT:
		printf("Formato incorrecto. El parametro -p espera .wav\n");
		break;
	case INVALID_STEG:
		printf("No es valido el algoritmo de esteganografiado\n");
		break;
	}
	exit(-1);
}
