#define MINIAUDIO_IMPLEMENTATION
#include "AudioPlayer.h"

#include <iostream>

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	/*
		In playback mode, copy data to pOutput. In capture mode, read data from pInput. In full-duplex mode, both
		pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
		frameCount frames.
	*/
}

static void InitializeAudioPlayer(ma_device_config& config, ma_device& device, ma_engine& engine, AudioDeviceSetup data)
{
	config = ma_device_config_init(data.ModeType);

	config.playback.format = ma_format_f32;
	config.playback.channels = 2;
	config.sampleRate = 48000;

	config.dataCallback = data_callback;
	
	device = ma_device();

	if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
	{
		std::cout << "AudioPlayer | Failed to initialize the device.\n";
		return;
	}

	ma_device_start(&device);

	ma_result result;
	engine = ma_engine();

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) std::cout << "AudioPlayer - InitializeAudioPlayer() | Failed to initialize the engine.\n";
}

AudioPlayer::AudioPlayer()
{
	AudioDeviceSetup data { };

	data.Channels = 2;

	data.FormatType = ma_format_f32;
	data.SampleRate = ma_standard_sample_rate_48000;

	data.ModeType = ma_device_type_playback;

	InitializeAudioPlayer(config, device, engine, data);
}

AudioPlayer::AudioPlayer(AudioDeviceSetup SetupData)
{
	InitializeAudioPlayer(config, device, engine, SetupData);
}

AudioPlayer::~AudioPlayer()
{
	ma_device_uninit(&device);
}