#pragma once
#include <al.h>
#include <alc.h>
#include <string>

class Audio
{
public:
	bool init();
	void cleanUp();
	
	ALuint loadWav(const char* filename);
	ALuint createSource(ALuint buffer, bool loop);

	void play(ALuint source);
	
	void deleteSource(ALuint source);
	void deleteBuffer(ALuint buffer);
private:
	ALCdevice* device;
	ALCcontext* context;
};