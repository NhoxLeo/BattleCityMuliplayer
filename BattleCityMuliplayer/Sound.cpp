#include "Sound.h"

bool Sound::Init(const WCHAR* file, bool Loop /* = false */)
{
	if (Loop)
	{
		//hm = BASS_StreamCreateFile(FALSE, file, 0, 0, BASS_SAMPLE_LOOP);
	}
	else
	{
		//hm = BASS_StreamCreateFile(FALSE, file, 0, 0, 0);
	}
	return true;
}

Sound* Sound::create(const WCHAR* file, bool Loop /* = false */)
{
	Sound* sound = new Sound();
	if (sound->Init(file, Loop))
	{
		return sound;
	}
	sound = NULL;
	return sound;
}

void Sound::Play()
{
	//BASS_ChannelPlay(hm, true);
}