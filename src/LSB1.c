#include "../include/LSB1.h"

static void hideBit(unsigned char *buffer, int size, unsigned char payload_bit);
static DWORD getSizeLSB1(FILE *fileptr, unsigned short int sample_size);
static void insertSizeLSB1(FILE *fileptr, FILE *outfile, unsigned short int sample_size, DWORD size);
static unsigned char lsb(unsigned char * buffer, int buffer_size);

unsigned char lsb(unsigned char * buffer, int buffer_size) {
  return (buffer[buffer_size - 1]) & 0x01;
}

DWORD getSizeLSB1(FILE * file, unsigned short int sample_bytes){
    unsigned char buffer[sample_bytes];
    int dword_bits = sizeof(DWORD) * 8;

    DWORD size = 0;
    unsigned char last_bit;
    for (int bits_read = 0; bits_read < dword_bits; bits_read++) {
        fread(buffer, 1, sample_bytes, file); // TODO: == -1?
        //agarro el bit menos sig de la muestra
        last_bit = lsb(buffer, sample_bytes);
        size <<= 1;
        size |= last_bit;
    }
    return size;
}

void insertSizeLSB1(FILE *fileptr, FILE *outfile, unsigned short int sample_size, DWORD size){
    //en cuantos bytes se guarda el tamaño
    int dword_size = sizeof(size);

    unsigned char *bytes = (unsigned char*)malloc(dword_size);
    memcpy(bytes, &size, dword_size);

    int bits_read = 0;
    int read = 0;
    unsigned char byte = bytes[0];  //agarro el primer byte del tamaño
    unsigned char bit;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);

    int bytes_read = 0;
    while (bytes_read < dword_size) {
        read = fread(buffer, 1, sample_size, fileptr);
        //agarro el bit payload
        bit = (byte >> (7 - bits_read)) & 0x01;
        //cambio el ultimo bit del buffer de lectura
        hideBit(buffer, read, bit);
        bits_read++;

        fwrite(buffer, 1, read, outfile);			// Writing read data into output file

        if (bits_read >= 8) { //si ya lei todo un byte agarro el que sigue
            bits_read = 0;
            bytes_read++;
            byte = bytes[bytes_read];
        }
    }
    free(bytes);
    free(buffer);
}

void hideLSB1(FILE * vector, FILE * out_file, unsigned char * payload, DWORD payload_size, unsigned short int sample_size){
    int read = 0;
    // int write = 0;
    unsigned int payload_read = 0;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);   //leo de a samples
    // unsigned char *payload_buffer = (unsigned char *)malloc(1);         //leo de a un byte
    unsigned char bit;
    int bits_read = 0;

    //inserto el size
    insertSizeLSB1(vector, out_file, sample_size, payload_size);

    //leo el primer B de la imagen
    //payload_read = fread(payload_buffer, 1, 1, payload);
    // unsigned char payloadByte = 0;

    //leo un sample a la vez
    while((read = fread(buffer, 1, sample_size, vector)) > 0) {
        if (bits_read >= 8) { //si ya lei todo un byte agarro el que sigue
            bits_read = 0;
            payload++;
            payload_read++;
        }
        if (payload_read < payload_size) {
            //agarro el bit bits_read
            bit = ((*payload) >> (7 - bits_read)) & 0x01;
            //cambio el ultimo bit del buffer de lectura
            hideBit(buffer, read, bit);
            bits_read++;
        }
        fwrite(buffer, 1, read, out_file);			// Writing read data into output file
    }
}

void hideBit(unsigned char *buffer, int size, unsigned char payload_bit){
    buffer[size-1] = ((buffer[size-1]) & ~1) | payload_bit;
    return;
}

void recoverLSB1(FILE * file, FILE * payload, unsigned short int sample_bytes){
    unsigned char * buffer = (unsigned char *)malloc(sample_bytes);
    unsigned char payload_buffer = 0;
    unsigned char last_bit;
    int bits_read = 0;

    DWORD payload_size = getSizeLSB1(file, sample_bytes);

    unsigned char * payload_buffer_file = (unsigned char *) malloc(payload_size);

    unsigned int bytes_recovered = 0;
    while (bytes_recovered < payload_size) {
        fread(buffer, 1, sample_bytes, file); // TODO: == -1 ?

        if (bits_read >= 8) { //si ya lei todo un byte agarro el que sigue
            bits_read = 0;
            memcpy(payload_buffer_file + bytes_recovered, &payload_buffer, 1);
            bytes_recovered++;
            payload_buffer = 0;
        }

        //agarro el ultimo bit de la muestra
        last_bit = lsb(buffer, sample_bytes);
        // last_bit = buffer[sample_bytes - 1] & 0x01;
        payload_buffer <<= 1;
        payload_buffer |= last_bit;

        bits_read++;
    }
    //desencriptar buffer
    //------------------
    fwrite(payload_buffer_file, payload_size, 1, payload);
    free(payload_buffer_file);
}
