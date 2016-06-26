#include "../include/marshall.h"

static int read_file(char ** buffer, char * filename);


char * marshall_plain(char * filename) {
  char * extension = strchr(filename, '.');
  char ** payload_buffer = (char **) malloc(sizeof(char **));

  int size_size = sizeof(DWORD);
  int extension_size = strlen(extension) + 1; // \0 has to be marshalled
  int payload_size = read_file(payload_buffer, filename);
  if (payload_size == -1) {
    return NULL;
  }
  int marshalled_size = size_size + payload_size + extension_size;

  char * marshalled_data = (char *) calloc(1, marshalled_size);
  sprintf(marshalled_data, "%d%s%s", payload_size, *payload_buffer, extension);

  free(*payload_buffer);
  free(payload_buffer);
  return marshalled_data;
}

char * marshall_encrypted(char * filename) {
  char ** payload_buffer = (char **) malloc(sizeof(char **));

  int payload_size = read_file(payload_buffer, filename);
  if (payload_size == -1) {
    return NULL;
  }
  int size_size = sizeof(DWORD);
  int marshalled_size = size_size + payload_size + 1; // \0 has to be marshalled

  char * marshalled_data = (char *) calloc(1, marshalled_size);
  sprintf(marshalled_data, "%d%s", payload_size, *payload_buffer);

  free(*payload_buffer);
  free(payload_buffer);
  return marshalled_data;
}

int read_file(char ** buffer, char * filename) {
  int length = -1;
  FILE * file = fopen(filename, "rb");

  if (file) {
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    *buffer = (char *) malloc(length + 1);
    if (*buffer) {
      fread(*buffer, 1, length, file);
      (*buffer)[length] = '\0';
    }
    fclose(file);
  }
  return length;
}
