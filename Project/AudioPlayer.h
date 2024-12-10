#pragma once

#include "miniaudio.h"

struct AudioDeviceSetup
{
public:
	int FormatType;

	ma_uint32 Channels;
	ma_uint32 SampleRate;

	ma_device_type ModeType;
};

class AudioPlayer
{
private:
	ma_device_config config;
	ma_device device;
	ma_engine engine;

public:
	AudioPlayer();
	AudioPlayer(AudioDeviceSetup SetupData);
	~AudioPlayer();

	void PlaySoundDirectly(const char* Sound) { ma_engine_play_sound(&engine, Sound, NULL); };
};