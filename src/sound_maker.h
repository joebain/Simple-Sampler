#ifndef _SOUND_MAKER_H_
#define _SOUND_MAKER_H_

class SoundMaker {
	public:
		virtual bool play() =0;
		virtual bool stop() =0;
		
		virtual bool is_playing() =0;
		virtual void next_frames(float frames[], int length) =0;
		float next_frame() {
			float data;
			next_frames(&data,1);
			return data;
		}
		
		int id;
		
		bool equal(SoundMaker* other) {
			return this->id == other->id;
		}
};

#endif
