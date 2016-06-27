#ifndef CRYPT_H
#define CRYPT_H

#include "../include/define.h"

typedef enum {
	UNKNOWN_ENC_TYPE, AES128, AES192, AES256, DES
} ENC_TYPE;

typedef enum {
	UNKNOWN_ENC_MODE, ECB, CFB, OFB, CBC
} ENC_MODE;

char * encrypt_buffer(char * buffer, ENC_TYPE type, ENC_MODE mode, char * password);
char * decrypt_buffer(char * buffer, ENC_TYPE type, ENC_MODE mode, char * password);

#define NON_ENC_PATH	"/tmp/.non_enc_file"
#define ENC_PATH 		 	"/tmp/.enc_file"

#endif // CRYPT_H
