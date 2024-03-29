#ifndef AUDIO_GENERATOR_H
#define AUDIO_GENERATOR_H

#include "../audio_player/audio_player.h"
#include "../audio_encoder/audio_encoder.h"

#include "../../3rdparty/openal/openal.h"

#include <cmath>
#include <exception>
#include <iostream>

#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_FORMAT AL_FORMAT_MONO16
#define DEFAULT_GENERATION_SIZE 1470

class audio_generator
{
public:
	audio_generator(int audio_sample_rate = DEFAULT_SAMPLE_RATE, int audio_format = DEFAULT_FORMAT, int generation_size = DEFAULT_GENERATION_SIZE);
	~audio_generator();
	void connect(audio_player* audio_player_block);
	void connect(audio_encoder* audio_encoder_block);
	void disconnect();
	void send();
	void set_generation_size(int generation_size);

	class internal_exception: public std::exception { };
private:
	int sample_rate;
	int format;
	int generation_size;
	double angular_frequency;

	int amplitude;
	int frequency;
	int time;

	int format_multiplier;
	char* buffer;

	audio_player* audio_player_block;
	audio_encoder* audio_encoder_block;
};

#endif // AUDIO_GENERATOR_H
