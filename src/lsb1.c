#include "../include/lsb1.h"

static void hide_bit(unsigned char *buffer, int size, unsigned char data_bit);
static unsigned char lsb(unsigned char * buffer, int buffer_bytes);
static int recover_bytes(unsigned char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read);

void print_bits(unsigned char num){
    unsigned char size = sizeof(unsigned char);
    unsigned char maxPow = 1 << (size * 8 - 1);

    printf("'");
    for(unsigned int i=0; i < size * 8; i++){
      // print last bit and shift left.
      printf("%u", (num & maxPow) ? 1 : 0);
      num <<= 1;
    }
    printf("'");
}

void hide_lsb1(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data) {
  int input_bytes_read = 0;
  int data_bits_read = 0;
  unsigned char data_bit;
  unsigned char * buffer = (unsigned char *) calloc(1, sizeof(char));

  int data_bytes_to_read = strlen(data); // TODO: get from struct!
  //leo un sample a la vez
  while((input_bytes_read = fread(buffer, BYTE_SIZE, sample_bytes, orig_file)) > 0) {
      if (data_bits_read >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
          data_bits_read = 0;
          data_bytes_to_read--;
          data++;
      }
      if (data_bytes_to_read > 0) {
          // agarro el bit mas significativo
          data_bit = ((*data) >> (7 - data_bits_read)) & 0x01;
          // cambio el ultimo bit del buffer de lectura
          hide_bit(buffer, sample_bytes, data_bit);
          data_bits_read++;
      }
      fwrite(buffer, BYTE_SIZE, input_bytes_read, vector); // Writing read data into output file
  }
}

void hide_bit(unsigned char * buffer, int buffer_size, unsigned char data_bit){
  int last_idx = buffer_size - 1;
  buffer[last_idx] = (buffer[last_idx] & ~0x01) | data_bit;
}

int recover_lsb1(FILE * data_file, FILE * vector, unsigned short int sample_bytes){
    DWORD data_size = 0;
    int bytes_recovered = recover_bytes((unsigned char *) &data_size, vector, sample_bytes, sizeof(DWORD));
    // printf("")
    if (bytes_recovered == -1) {
      return -1;
    }

    unsigned char * data = (unsigned char *) malloc(data_size);
    bytes_recovered += recover_bytes(data, vector, sample_bytes, data_size);
    if (bytes_recovered == -1) {
      return -1;
    }

    fwrite(data, data_size, 1, data_file);
    free(data);
    return bytes_recovered;
}

int recover_bytes(unsigned char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read) {
  unsigned char * vector_buffer = (unsigned char *) calloc(1, sizeof(char));
  unsigned char data_byte = 0;

  unsigned int bytes_read = 0;
  unsigned int bits_read = 0;

  while (bytes_read < bytes_to_read) {
      fread(vector_buffer, 1, sample_bytes, vector); // TODO: == -1 ?

      if (bits_read >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
          data[bits_read / BITS_PER_BYTE] = data_byte;
          data_byte = 0;
          bits_read = 0;
          bytes_read++;
      }

      // agarro el ultimo bit de la muestra
      data_byte <<= 1;
      data_byte |= lsb(vector_buffer, sample_bytes);

      bits_read++;
  }
  if (bits_read * BITS_PER_BYTE < bytes_to_read) {
    return -1;
  }
  return bytes_to_read;
}

unsigned char lsb(unsigned char * buffer, int buffer_bytes) {
  return (buffer[buffer_bytes - 1]) & 0x01;
}
