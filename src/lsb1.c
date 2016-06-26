#include "../include/lsb1.h"

static void hide_bit(unsigned char *buffer, int size, unsigned char data_bit);
static unsigned char lsb(unsigned char * buffer, int buffer_bytes);
static int recover_bytes(char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read);
// static void print_bits(unsigned char num);
//
// void print_bits(unsigned char num){
//     unsigned char size = sizeof(unsigned char);
//     unsigned char maxPow = 1 << (size * 8 - 1);
//
//     printf("'");
//     for(unsigned int i=0; i < size * 8; i++){
//       // print last bit and shift left.
//       printf("%u", (num & maxPow) ? 1 : 0);
//       num <<= 1;
//     }
//     printf("'");
// }

void hide_lsb1(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data, unsigned int bytes_to_hide) { // TODO: unsigned long
  int input_bytes_read = 0;
  int bits_hidden = 0;
  unsigned char bit_to_hide;
  unsigned char * buffer = (unsigned char *) calloc(1, sizeof(char));

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

int recover_lsb1(char * base_out_path, FILE * vector, unsigned short int sample_bytes){
    // load body size
    DWORD data_size = 0;
    int bytes_recovered = recover_bytes((char *) &data_size, vector, sample_bytes, sizeof(DWORD));
    data_size = __builtin_bswap64(data_size);

    // load body
    char * data = (char *) calloc(data_size, sizeof(char));
    bytes_recovered += recover_bytes(data, vector, sample_bytes, data_size);

    // load extension
    char extension[MAX_EXT_LEN + 1] = { '\0' };
    int i = 0;
    char ext_c = 0;
    do {
      recover_bytes(&ext_c, vector, sample_bytes, sizeof(char));
      extension[i] = ext_c;
      i++;
    } while(ext_c != '\0');

    // save all to new file
    char * filename = (char *) calloc(strlen(base_out_path) + strlen(extension) + 1, sizeof(char));
    sprintf(filename, "%s%s", base_out_path, extension);

    FILE * data_file = fopen(filename, "wb+");
    fwrite(data, data_size, 1, data_file);

    free(filename);
    free(data);
    return bytes_recovered;
}

int recover_bytes(char * data, FILE * vector, unsigned short int sample_bytes, unsigned int bytes_to_read) {
  unsigned char * vector_buffer = (unsigned char *) calloc(sample_bytes, BYTE_SIZE);
  unsigned char data_byte = 0;

  unsigned int bytes_read = 0;
  unsigned int bits_read = 0;

  printf("\nbytes_to_read: %d\n", bytes_to_read);
  while(bytes_read < bytes_to_read) {
      fread(vector_buffer, 1, sample_bytes, vector); // TODO: == -1 ?

      data_byte <<= 1;
      data_byte |= lsb(vector_buffer, sample_bytes);

      bits_read++;

      if (bits_read == BITS_PER_BYTE) { //si ya lei todo un byte agarro el que sigue
          data[bytes_read] = data_byte;
          data_byte = 0;
          bits_read = 0;
          bytes_read++;
      }
  }
  free(vector_buffer);
  return bytes_to_read;
}

unsigned char lsb(unsigned char * buffer, int buffer_bytes) {
  return (buffer[buffer_bytes - 1]) & 0x01;
}
