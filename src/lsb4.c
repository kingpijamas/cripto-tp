
#include "../include/lsb4.h"

void hideBitsLSB4(unsigned char *buffer, int size, unsigned char img_bit[]) {
    if (img_bit[0] == 1) {
        buffer[size - 1] |= 1;
    } else {
        buffer[size - 1] &= ~1;
    }

    if (img_bit[1] == 1) {
        buffer[size - 1] |= 2;
    } else {
        buffer[size - 1] &= ~2;
    }

    if (img_bit[2] == 1) {
        buffer[size - 1] |= 4;
    } else {
        buffer[size - 1] &= ~4;
    }

    if (img_bit[3] == 1) {
        buffer[size - 1] |= 8;
    } else {
        buffer[size - 1] &= ~8;
    }
    return;
}

DWORD getSizeLSB4(FILE *fileptr, unsigned short int sample_size) {
    unsigned char buffer[sample_size];
    int dword_size = sizeof(DWORD);

    DWORD size = 0;
    int index = 0;
    int read = 0;
    unsigned char bits[4];
    int i = 0;
    while (i < dword_size) {
        read = fread(buffer, 1, sample_size, fileptr);
        //agarro el bit menos sig de la musetra
        bits[0] = ((buffer[sample_size - 1]) >> 0) & 1;
        bits[1] = ((buffer[sample_size - 1]) >> 1) & 1;
        bits[2] = ((buffer[sample_size - 1]) >> 2) & 1;
        bits[3] = ((buffer[sample_size - 1]) >> 3) & 1;

        if ((bits[0] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        if ((bits[1] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        if ((bits[2] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;
        if ((bits[3] & 0x01) == 1) {    //si es un uno, seteoun uno en el bit index de ese byte
            size |= 1 << index;
        }
        index++;

        if (index % 8 == 0) { //si ya lei todo un byte incremento el tamaño
            i++;
        }
    }
    return size;
}

void insertSizeLSB4(FILE *fileptr, FILE *outfile, unsigned short int sample_size, DWORD size) {
    //en cuantos bytes se guarda el tamaño
    int dword_size = sizeof(size);

    unsigned char *bytes = (unsigned char*)malloc(dword_size);
    memcpy(bytes, &size, dword_size);

    int index = 0;
    int read = 0;
    unsigned char byte = bytes[0];  //agarro el primer byte del tamaño
    unsigned char bits[4];
    unsigned char *buffer = (unsigned char *)malloc(sample_size);

    int i = 0;
    while (i < dword_size) {
        read = fread(buffer, 1, sample_size, fileptr);
        //agarro el bit index
        bits[0] = (byte >> index++) & 1;
        bits[1] = (byte >> index++) & 1;
        bits[2] = (byte >> index++) & 1;
        bits[3] = (byte >> index++) & 1;
        //printf("%u %u %u %u", bits[0], bits[1], bits[2], bits[3]);
        //cambio el ultimo bit del buffer de lectura
        hideBitsLSB4(buffer, read, bits);

        fwrite(buffer, 1, read, outfile);			// Writing read data into output file

        if (index >= 8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            byte = bytes[++i];
        }
    }
    free(bytes);
    free(buffer);
}

void hide_lsb4(FILE * vector, FILE * orig_file, unsigned short int sample_bytes, char * data) {
  int input_bytes_read = 0;
  int bits_hidden = 0;
  unsigned char bits_to_hide;
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
          bits_to_hide = ((*data) >> (4 - bits_hidden)) & 0x0F;
          // cambio el ultimo bit del buffer de lectura
          // hide_bit(buffer, sample_bytes, bits_to_hide);
          int last_idx = sample_bytes - 1;
          buffer[last_idx] = (buffer[last_idx] & 0xF0) | bits_to_hide;
          bits_hidden += 4;
      }
      fwrite(buffer, BYTE_SIZE, input_bytes_read, vector); // Writing read data into output file
  }
}

void recover_lsb4(FILE *fileptr, FILE *img_out, unsigned short int sample_size) {
    int read = 0;
    int img_read = 0;
    //unsigned short int sample_size = header.bits_per_sample / 8;
    unsigned char *buffer = (unsigned char *)malloc(sample_size);
    unsigned char img_buffer[1];
    unsigned char bits[4];
    int index = 0;

    img_buffer[0] = 0;


    DWORD size = getSizeLSB4(fileptr, sample_size);

    unsigned char *img_buffer_file = (unsigned char *)malloc(size);

    int i = 0;
    while(i < size)
    {
        read = fread(buffer, 1, sample_size, fileptr);

        if (index >= 8) { //si ya lei todo un byte agarro el que sigue
            index = 0;
            memcpy(img_buffer_file+(i++), img_buffer, 1);
            img_buffer[0] = 0;
        }
        //agarro el ultimo bit de la muestra
        bits[0] = ((buffer[sample_size - 1]) >> 0) & 1;
        bits[1] = ((buffer[sample_size - 1]) >> 1) & 1;
        bits[2] = ((buffer[sample_size - 1]) >> 2) & 1;
        bits[3] = ((buffer[sample_size - 1]) >> 3) & 1;

        if ((bits[0] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
        if ((bits[1] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
        if ((bits[2] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
        if ((bits[3] & 0x01) == 1) {    //si es un uno, seteo el uno en el bit index de ese byte
            img_buffer[0] |= 1 << index;
        }
        index++;
    }
    //desencriptar buffer
    //------------------
    fwrite(img_buffer_file, size, 1, img_out);
    free(img_buffer_file);
}
