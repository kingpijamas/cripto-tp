#define OK 0
#define INVALID_OP 1
#define MISSING_IN 2
#define MISSING_P 3
#define INVALID_IN_FORMAT 4
#define MISSING_IN_FILE 5
#define MISSING_P_FILE 6
#define INVALID_P_FORMAT 7
#define MISSING_OUT 8
#define MISSING_OUT_FILE 9
#define INVALID_OUT_FORMAT 10
#define MISSING_STEG 11
#define INVALID_STEG 12
#define INVALID_A 13

#define NUM_ENC 4
#define STRING_SIZE 10

typedef enum {
  LSB1,
  LSB4,
  LSBE
} StegType;

typedef enum {
  aes128,
  aes192,
  aes256,
  des
} EncType;

//typedef char enc[NUM_ENC][NUM_ENC];

// char *enc[NUM_ENC];
// enc[0] = "aes128";
// enc[1] = "aes192";
// enc[2] = "aes256";
// enc[3] = "des";