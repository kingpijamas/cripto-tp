#include "../include/stegoanalisis.h"

void stegoanalyze( p_path){
	printf("To do this stegoanalisis");
	FILE * vector = fopen(p_path, "rb");
	WAV_HEADER header = parse_header(vector);
	int bytes_per_sample = header.bits_per_sample / BITS_PER_BYTE;
//	testLSB1(vector, bytes_per_sample);
//	testLSB4(vector, bytes_per_sample);
//	testLSBEnh(vector, bytes_per_sample);
}

//void testLSB1(FILE * vector, bytes_per_sample){
//
//}
