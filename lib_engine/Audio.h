#pragma once
#include <map>
#include <string>

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/Music.hpp"

class Audio
{
public:

	// You should specify volume in the range 0 to 1, where 0 is silent and 1 is full volume.
	// You should specify pitch in the range 0 to 1, where 0.5 is half-pitch (lower/deeper) and 1.0 is normal.

	static void Sound_Load_from_file(const std::string& file_name, const std::string& sound_name);
	static void Sound_Play(const std::string& sound_name, float volume, float pitch);
	static void Sound_Play_Looping(const std::string& sound_name, float volume, float pitch);
	static void Sound_Stop(const std::string& sound_name);
	static void Sound_Set_Volume(const std::string& sound_name, float volume);
	static void Sound_Set_Pitch(const std::string& sound_name, float pitch);
	static void Sound_Stop_all_sounds();
	static void Sound_Adjust_volume_of_all_sounds(float coefficient);
	static void Sound_Adjust_pitch_of_all_sounds(float coefficient);


	static void Music_Load_from_file(const std::string file_name, const std::string music_name);
	static void Music_Play(const std::string music_name, float volume, float pitch);
	static void Music_Stop(const std::string music_name);
	static void Music_Set_Volume(const std::string music_name, float coefficient);
	static void Music_Set_Pitch(const std::string music_name, float coefficient);
	static void Music_Stop_all_music();
	static void Music_Adjust_volume_of_all_musics(float coefficient);

	static void UnloadAll();
	/*
	static unsigned long long Music_Load_from_file(const std::string file_name);
	static void Music_Play(const int track_index, float volume, float pitch);
	static void Music_Stop(const int track_index);
	static void Music_Set_Volume(const int track_index, float coefficient);
	static void Music_Set_Pitch(const int track_index, float coefficient);
	*/
protected:
	static std::map<std::string, sf::SoundBuffer> SoundBuffers;
	static std::map<std::string, sf::Sound> Sounds;

	// It is illegal to copy a sf::Music, so we must work only with pointers, but we also lose the music and crash at runtime if it is destroyed by going out of scope.
	static std::map<std::string, std::unique_ptr<sf::Music>> Musics;
	// static std::vector<sf::Music> Musics;
};

