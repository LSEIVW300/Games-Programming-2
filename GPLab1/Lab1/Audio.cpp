#include "Audio.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

bool Audio::init()
{
	device = alcOpenDevice(NULL);
	if (!device) return false;

	context = alcCreateContext(device, NULL);
	if (!context) return false;

	alcMakeContextCurrent(context);
	return true;
}

void Audio::cleanUp()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

ALuint Audio::loadWav(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);

	if (!file)
	{
		std::cout << "Failed to open wav: " << filename << std::endl;
		return 0;
	}

	char riff[4];
	file.read(riff, 4);

	if (strncmp(riff, "RIFF", 4) != 0)
	{
		std::cout << "Not a valid RIFF file: " << filename << std::endl;
		return 0;
	}

	file.ignore(4);

	char wave[4];
	file.read(wave, 4);

	if (strncmp(wave, "WAVE", 4) != 0)
	{
		std::cout << "Not a valid WAVE file: " << filename << std::endl;
		return 0;
	}

	short audioFormat = 0;
	short channels = 0;
	int sampleRate = 0;
	short bitsPerSample = 0;

	std::vector<char> data;
	int dataSize = 0;

	while (!file.eof())
	{
		char chunkId[4];
		int chunkSize = 0;

		file.read(chunkId, 4);
		file.read((char*)&chunkSize, 4);

		if (file.eof())
			break;

		if (strncmp(chunkId, "fmt ", 4) == 0)
		{
			file.read((char*)&audioFormat, 2);
			file.read((char*)&channels, 2);
			file.read((char*)&sampleRate, 4);

			file.ignore(6);

			file.read((char*)&bitsPerSample, 2);

			if (chunkSize > 16)
				file.ignore(chunkSize - 16);
		}
		else if (strncmp(chunkId, "data", 4) == 0)
		{
			dataSize = chunkSize;

			if (dataSize <= 0 || dataSize > 50000000)
			{
				std::cout << "Invalid WAV data size: " << dataSize << std::endl;
				return 0;
			}

			data.resize(dataSize);
			file.read(data.data(), dataSize);
		}
		else
		{
			file.ignore(chunkSize);
		}
	}

	ALenum format = 0;

	if (channels == 1 && bitsPerSample == 8)
		format = AL_FORMAT_MONO8;
	else if (channels == 1 && bitsPerSample == 16)
		format = AL_FORMAT_MONO16;
	else if (channels == 2 && bitsPerSample == 8)
		format = AL_FORMAT_STEREO8;
	else if (channels == 2 && bitsPerSample == 16)
		format = AL_FORMAT_STEREO16;
	else
	{
		std::cout << "Unsupported WAV format: " << filename << std::endl;
		return 0;
	}

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data.data(), dataSize, sampleRate);

	std::cout << "Loaded WAV: " << filename << std::endl;

	return buffer;
}

ALuint Audio::createSource(ALuint buffer, bool loop)
{
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	alSourcef(source, AL_GAIN, 0.7f);

	return source;
}

void Audio::play(ALuint source)
{
	alSourcePlay(source);
}

void Audio::deleteSource(ALuint source)
{
	if (source != 0)
		alDeleteSources(1, &source);
}

void Audio::deleteBuffer(ALuint buffer)
{
	if (buffer != 0)
		alDeleteBuffers(1, &buffer);
}