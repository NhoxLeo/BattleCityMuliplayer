#ifndef SOUND_H
#define SOUND_H

#include "common.h"

class Sound
{
	//HMUSIC hm;
public:
	bool Init(const WCHAR* file, bool Loop = false);
	static Sound* create(const WCHAR* file, bool Loop = false);
	void Play();
	~Sound(){}
};

#endif