#ifndef _SOUND_MAKER_H_
#define _SOUND_MAKER_H_

class SoundMaker {
	public:
		virtual bool play() =0;
		virtual bool stop() =0;
		
		virtual bool is_playing() =0;
		virtual void next_frames(float frames[], int length) =0;
		
		int id;
		
		bool equal(SoundMaker* other) {
			return this->id == other->id;
		}
};

#endif
