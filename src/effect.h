#ifndef _EFFECT_H_
#define _EFFECT_H_

class Effect {
	private:
		int id;
	public:
		Effect(int id) { this->id = id;}
		virtual void process(float frames[], int length) =0;
		bool equal(Effect * other) {return this->id == other->id;};
};

#endif
