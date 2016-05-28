//
//  Encode.c
//  
//
//  Created by Julieta Sal-lari on 28/5/16.
//
//

#include "Encode.h"

void encode(unsigned char *buffer, int size, unsigned char img_bit){
    
    (*(buffer+(size-1))) = ((*(buffer+(size-1))) & ~1) | img_bit;
    
    return;
}