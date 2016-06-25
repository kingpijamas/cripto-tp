#define DEFAULT_MODE		    "cbc"
#define DEFAULT_ENCTYPE     "aes128"

#define WAV_EXT					    ".wav"
#define BMP_EXT             ".bmp"

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

#define NUM_ENC              4
#define STRING_SIZE         10

typedef enum {
	true, false
} bool;
