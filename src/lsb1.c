#include "../include/lsb1.h"

static void hide_bit(unsigned char *buffer, int size, unsigned char data_bit);
static unsigned char lsb(unsigned char * buffer, int buffer_bytes);
static int recover_bytes(unsigned char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read);

void hide_lsb1(FILE * vector, FILE * in_file, unsigned short int sample_bytes, char * data) {
  int input_bytes_read = 0;
  int data_bits_read = 0;
  unsigned char data_bit;
  unsigned char * buffer = (unsigned char *) calloc(1, sizeof(char));

  //leo un sample a la vez
  while((input_bytes_read = fread(buffer, 1, sample_bytes, in_file)) > 0) {
      if (data_bits_read >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
          data_bits_read = 0;
          data++;
          // data_read++;
      }
      if (*data != '\0') { //(data_read < data_size) {
          // agarro el bit mas significativo
          data_bit = ((*data) >> (7 - data_bits_read)) & 0x01;
          // cambio el ultimo bit del buffer de lectura
          hide_bit(buffer, sample_bytes, data_bit);
          data_bits_read++;
      }
      fwrite(buffer, 1, input_bytes_read, vector); // Writing read data into output file
  }
}

void hide_bit(unsigned char * buffer, int buffer_size, unsigned char data_bit){
  int last_idx = buffer_size - 1;
  // printf("\n%d->", buffer[last_idx]);
  buffer[last_idx] = (buffer[last_idx] & ~0x01) | data_bit;
  // printf("%d\n", buffer[last_idx]);
}

int recover_lsb1(FILE * data_file, FILE * vector, unsigned short int sample_bytes){
    DWORD data_size = 0;
    int bytes_recovered = recover_bytes((unsigned char *) &data_size, vector, sample_bytes, sizeof(DWORD));
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

  unsigned int bits_to_read = bytes_to_read * BITS_PER_BYTE;
  unsigned int bits_read = 0;
  while (bits_read < bits_to_read) {
      fread(vector_buffer, 1, sample_bytes, vector); // TODO: == -1 ?

      if (bits_read >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
          data[bits_read / BITS_PER_BYTE] = data_byte;
          bits_read = 0;
          data_byte = 0;
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
