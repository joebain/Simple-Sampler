#ifndef _BIT_EFFECT_H_
#define _BIT_EFFECT_H_

#include "effect.h"

class BitEffect : public Effect
{
	private:
		
	public:
		BitEffect(int id);
		
		float quant;
		float offset;
		
		char off[8];
		char flip[8];
		
		void process(float frames[], int length);
};

#endif
