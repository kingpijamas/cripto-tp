//  crypt.h

#ifndef MAIN_H
#define MAIN_H

#include "../include/LSB1.h"
#include "../include/LSB4.h"
#include "../include/LSBEnhanced.h"
#include "../include/crypt.h"
#include "define.h"

#define WAV_EXT ".wav"
#define BMP_EXT ".bmp"

typedef enum {
	NO_COMMAND, ANALYZE, EMBED, EXTRACT
} comm;

typedef enum {
	UNKNOWN_ARG, EMBED_ARG, EXTRACT_ARG, IN_ARG, P_ARG, OUT_ARG, STEG_ARG, A_ARG, M_ARG, PASS_ARG
} arg;

#define SYS_OK 0

typedef enum {
	INVALID_OP,
	MISSING_P,
	MISSING_IN_FILE,
	MISSING_P_FILE,
	INVALID_P_FORMAT,
	MISSING_OUT_FILE,
	INVALID_OUT_FORMAT,
	MISSING_STEG,
	INVALID_STEG,
	INVALID_A,
	INVALID_M,
	MISSING_PASS,
	INC_PARAMC
} error;

static int fileExists(char * path);
static int streq(char * str1, char * str2);
static int empty(char * str);
static int validStegAlgorithm(char * algorithm);
static enc_type parse_enc_type(char * type);
static enc_mode parse_enc_mode(char * mode);
static int hasSuffix(char * str, char * suffix);
static arg parseArg(char * argument);
static void expectFileToExist(char * path, error error_code);
static void expectSuffix(char * param, char * suffix, error error_code);
static void fail(error code, char * param);
static void print_help();

#endif // CRYPT_H
