#include "bit_effect.h"

#include <stdlib.h>
#include <iostream>

BitEffect::BitEffect(int id) : Effect(id) {
	quant = 8;
	offset = 0.0;
}

void BitEffect::process(float frames[], int length) {
	
	//turn bits off
	unsigned int off_int = 0;
	for (int m = 0 ; m < 8 ; m++)
		off_int |= off[m]<<m;
	off_int = ~off_int;
	
	//flip bits
	unsigned int flip_int = 0;
	for (int m = 0 ; m < 8 ; m++)
		flip_int |= flip[m]<<m;
	
	for (int i = 0 ; i < length ; i++) {
		frames[i] -= offset;
		int tmp = (int)(frames[i]*quant);
		
		//apply bit mangling
		tmp &= off_int;
		tmp ^= flip_int;
		
		frames[i] = ((float)tmp)/quant;
		frames[i] += offset;
	}
}
