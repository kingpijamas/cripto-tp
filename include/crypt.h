//  crypt.h

#ifndef CRYPT_H
#define CRYPT_H

#include "../include/define.h"

typedef enum {
	UNKNOWN_ENC_TYPE, AES128, AES192, AES256, DES
} enc_type;

typedef enum {
	UNKNOWN_ENC_MODE, ECB, CFB, OFB, CBC
} enc_mode;

char * encrypt_buffer(char * buffer, enc_type type, enc_mode mode, char * password);

#define NON_ENC_PATH "/tmp/.non_enc_file"
#define ENC_PATH "/tmp/.enc_file"

#endif // CRYPT_H
