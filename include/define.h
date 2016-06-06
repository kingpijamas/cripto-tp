#define NO_COMMAND 			    0
#define ANALYZE   		 	    1
#define EMBED 	   			    2
#define EXTRACT    			    3

#define DEFAULT_MODE		    "cbc"
#define DEFAULT_ENCTYPE     "aes128"

#define WAV_EXT					    ".wav"
#define BMP_EXT             ".bmp"

#define OK                   0
#define INVALID_OP           1
#define MISSING_IN           2
#define MISSING_P            3
#define INVALID_IN_FORMAT    4
#define MISSING_IN_FILE      5
#define MISSING_P_FILE       6
#define INVALID_P_FORMAT     7
#define MISSING_OUT          8
#define MISSING_OUT_FILE     9
#define INVALID_OUT_FORMAT  10
#define MISSING_STEG        11
#define INVALID_STEG        12
#define INVALID_A           13
#define INVALID_M           14
#define MISSING_PASS        15

#define NUM_ENC              4
#define STRING_SIZE         10

enum bool {
  true = 1, false = 0;
}
