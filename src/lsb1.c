#include "../include/lsb1.h"

static void hide_bit(unsigned char *buffer, int size, unsigned char data_bit);
static unsigned char lsb(unsigned char * buffer, int buffer_bytes);
static int recover_bytes(unsigned char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read);
static void print_bits(unsigned char num);

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
  int bits_hidden = 0;
  unsigned char bit_to_hide;
  unsigned char * buffer = (unsigned char *) calloc(1, sizeof(char));

  printf("\ndata: %s\n\n", data);
  int bytes_to_hide = strlen(data); // TODO: get from struct!
  //leo un sample a la vez
  while((input_bytes_read = fread(buffer, BYTE_SIZE, sample_bytes, orig_file)) > 0) {
      if (bits_hidden >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
          bits_hidden = 0;
          bytes_to_hide--;
          data++;
      }
      if (bytes_to_hide > 0) {
          // agarro el bit mas significativo
          bit_to_hide = ((*data) >> (7 - bits_hidden)) & 0x01;
          // cambio el ultimo bit del buffer de lectura
          hide_bit(buffer, sample_bytes, bit_to_hide);
          bits_hidden++;
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
    printf("bytes_recovered: %lu, data_size: %.*s vs %lu\n", bytes_recovered, (int) sizeof(DWORD), (unsigned char *) &data_size, data_size);

    unsigned char * data = (unsigned char *) malloc(data_size);
    bytes_recovered += recover_bytes(data, vector, sample_bytes, data_size);

    fwrite(data, data_size, 1, data_file);
    free(data);
    return bytes_recovered;
}

int recover_bytes(unsigned char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read) {
  unsigned char * vector_buffer = (unsigned char *) calloc(sample_bytes, BYTE_SIZE);
  unsigned char data_byte = 0;

  unsigned int bytes_read = 0;
  unsigned int bits_read = 0;

  while(bytes_read < bytes_to_read) {
      fread(vector_buffer, BYTE_SIZE, sample_bytes, vector); // TODO: == -1 ?

      if (bits_read >= BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue

          // printf("data_size: %lu->", (DWORD) *data);
          print_bits(data[bytes_read]);
          data[bytes_read] = data_byte;
          printf(" -> ");
          print_bits(data[bytes_read]);
          printf("\n");
          // printf("%s\n", data);
          printf("'%.*s'\n", sizeof(DWORD), data);
          // printf("%lu\n", (DWORD) *data);
          data_byte = 0;
          bits_read = 0;
          bytes_read++;
          // printf("--------------");
      }

      // printf("\n\n");
      // print_bits(vector_buffer[sample_bytes - 1]);
      // agarro el ultimo bit de la muestra
      data_byte <<= 1;
      data_byte |= lsb(vector_buffer, sample_bytes);
      // printf("->");
      // print_bits(data_byte);
      // printf("\n");

      bits_read++;
  }
  free(vector_buffer);
  return bytes_to_read;
}

unsigned char lsb(unsigned char * buffer, int buffer_bytes) {
  return (buffer[buffer_bytes - 1]) & 0x01;
}
