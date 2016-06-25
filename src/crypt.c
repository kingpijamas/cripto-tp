#include "../include/crypt.h"

char * encrypt_buffer(char * buffer, enc_type type, enc_mode mode, char * password) {
	FILE * non_encrypted_file = fopen(NON_ENC_PATH, "wb");
	if (non_encrypted_file) {
		fwrite(buffer, strlen(buffer) + 1, 1, non_encrypted_file);
	} else {
		printf("Error encrypting file.");
		exit(1);
	}
	fclose(non_encrypted_file);
	char command[120];
	strcpy(command, "openssl enc ");
	switch (type) {
	when(AES128)
		strcat(command, "-aes-128");
		break;
	when(AES192)
		strcat(command, "-aes-192");
		break;
	when(AES256)
		strcat(command, "-aes-256");
		break;
	when(DES)
		strcat(command, "-des");
		break;
	}
	switch (mode) {
	when(ECB)
		strcat(command, "-ecb");
		break;
	when(CFB)
		strcat(command, "-cfb");
		break;
	when(OFB)
		strcat(command, "-ofb");
		break;
	when(CBC)
		strcat(command, "-cbc");
		break;
	}
	strcat(command, " -pass pass:");
	strcat(command, password);
	strcat(command, " -in ");
	strcat(command, NON_ENC_PATH);
	strcat(command, " -out ");
	strcat(command, ENC_PATH);
	system(command);
	memset(command, 0, sizeof(command));
	strcat(command, "rm ");
	strcat(command, NON_ENC_PATH);
	return ENC_PATH;
}

char * decrypt_buffer(char * buffer, enc_type type, enc_mode mode, char * password) {
	FILE * encrypted_file = fopen(ENC_PATH, "wb");
	if (encrypted_file) {
		fwrite(buffer, strlen(buffer) + 1, 1, encrypted_file);
	} else {
		printf("Error decrypting file.");
		exit(1);
	}
	fclose(encrypted_file);
	char command[120];
	strcpy(command, "openssl enc -d ");
	switch (type) {
	when(AES128)
		strcat(command, "-aes-128");
		break;
	when(AES192)
		strcat(command, "-aes-192");
		break;
	when(AES256)
		strcat(command, "-aes-256");
		break;
	when(DES)
		strcat(command, "-des");
		break;
	}
	switch (mode) {
	when(ECB)
		strcat(command, "-ecb");
		break;
	when(CFB)
		strcat(command, "-cfb");
		break;
	when(OFB)
		strcat(command, "-ofb");
		break;
	when(CBC)
		strcat(command, "-cbc");
		break;
	}
	strcat(command, " -pass pass:");
	strcat(command, password);
	strcat(command, " -in ");
	strcat(command, ENC_PATH);
	strcat(command, " -out ");
	strcat(command, NON_ENC_PATH);
	system(command);
	memset(command, 0, sizeof(command));
	strcat(command, "rm ");
	strcat(command, ENC_PATH);
	return *command;
}
