//  crypt.h

#ifndef MAIN_H
#define MAIN_H

#include "define.h"
#include "crypt.h"
#include "lsb1.h"
#include "lsb4.h"
#include "lsb_enhanced.h"
#include "marshall.h"
#include "wav_header_utils.h"

#define WAV_EXT ".wav"
#define BMP_EXT ".bmp"

typedef enum {
	NO_COMMAND, ANALYZE, EMBED, EXTRACT
} comm;

typedef enum {
	UNKNOWN_ARG, EMBED_ARG, EXTRACT_ARG, IN_ARG, P_ARG, OUT_ARG, STEG_ARG, A_ARG, M_ARG, PASS_ARG
} arg;

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

#define SYS_OK 0

#endif // CRYPT_H
