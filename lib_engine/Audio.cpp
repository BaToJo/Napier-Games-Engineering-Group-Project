#include "Audio.h"


using namespace std;
using namespace sf;

// This implementation of sound and music in SFML was made with reference to https://www.sfml-dev.org/tutorials/2.5/audio-sounds.php (SFML, accessed 2023).

std::map<std::string, sf::SoundBuffer> Audio::SoundBuffers;
std::map<std::string, sf::Sound> Audio::Sounds;
std::map<std::string, std::unique_ptr<sf::Music>> Audio::Musics;
// std::vector<sf::Music> Audio::Musics;


void Audio::Sound_Load_from_file(const std::string& file_name, const std::string& sound_name)
{
	// If we have already loaded this sound, exit immediately so we don't load a duplicate.
	if (SoundBuffers.contains(sound_name))
	{
		throw std::invalid_argument("Tried to overwrite an existing sound of the same name!");
		return;
	}

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(file_name))
	{
		throw std::invalid_argument("Tried to load a sound file that wasn't valid!");
		return;
	}

	sf::Sound sound;
	sound.setBuffer(buffer);

	SoundBuffers.emplace(sound_name, buffer);
	Sounds.emplace(sound_name, sound);
}

void Audio::Sound_Play(const std::string& sound_name, float volume, float pitch)
{
	if (!Sounds.contains(sound_name))
	{
		throw std::invalid_argument("No sound of that name has been loaded. Have you loaded the sound from file in advance properly?");
		return;
	}
	Sounds.find(sound_name)->second.setLoop(false);
	Sounds.find(sound_name)->second.setVolume(volume * 100.0f);
	Sounds.find(sound_name)->second.setPitch(pitch);
	Sounds.find(sound_name)->second.setBuffer(SoundBuffers.find(sound_name)->second);
	Sounds.find(sound_name)->second.play();
}
void Audio::Sound_Play_Looping(const std::string& sound_name, float volume, float pitch)
{
	if (!Sounds.contains(sound_name))
	{
		throw std::invalid_argument("No sound of that name has been loaded. Have you loaded the sound from file in advance properly?");
		return;
	}
	Sounds.find(sound_name)->second.setLoop(true);
	Sounds.find(sound_name)->second.setVolume(volume * 100.0f);
	Sounds.find(sound_name)->second.setPitch(pitch);
	Sounds.find(sound_name)->second.setBuffer(SoundBuffers.find(sound_name)->second);
	Sounds.find(sound_name)->second.play();
}

void Audio::Sound_Stop(const std::string& sound_name)
{
	if (!Sounds.contains(sound_name))
	{
		throw std::invalid_argument("No sound of that name has been loaded. Have you loaded the sound from file in advance properly?");
		return;
	}
	Sounds.find(sound_name)->second.stop();
}

void Audio::Sound_Set_Volume(const std::string& sound_name, float volume)
{
	if (!Sounds.contains(sound_name))
	{
		throw std::invalid_argument("No sound of that name has been loaded. Have you loaded the sound from file in advance properly?");
		return;
	}
	if (volume < 0)
	{
		volume = 0;
	}
	else if (volume > 1)
	{
		volume = 1;
	}
	Sounds.find(sound_name)->second.setVolume(volume * 100.0f);
}

void Audio::Sound_Set_Pitch(const std::string& sound_name, float pitch)
{
	if (!Sounds.contains(sound_name))
	{
		throw std::invalid_argument("No sound of that name has been loaded. Have you loaded the sound from file in advance properly?");
		return;
	}
	if (pitch < 0)
	{
		pitch = 0;
	}
	else if (pitch > 1)
	{
		pitch = 1;
	}
	Sounds.find(sound_name)->second.setPitch(pitch);
}

void Audio::Sound_Stop_all_sounds()
{
	for (auto& sound : Sounds)
	{
		sound.second.stop();
	}
}

void Audio::Sound_Adjust_volume_of_all_sounds(float coefficient)
{
	for (auto& sound : Sounds)
	{
		sound.second.setVolume(sound.second.getVolume() * coefficient);
	}
}

void Audio::Sound_Adjust_pitch_of_all_sounds(float coefficient)
{
	for (auto& sound : Sounds)
	{
		sound.second.setPitch(sound.second.getPitch() * coefficient);
	}
}


void Audio::Music_Load_from_file(const std::string file_name, const std::string music_name)
{
	// If we have already loaded this music, exit immediately so we don't load a duplicate.
	if (Musics.contains(music_name))
	{
		throw std::invalid_argument("Tried to overwrite an existing music of the same name!");
		return;
	}
	std::unique_ptr<sf::Music> music = make_unique<sf::Music>();
	if (!music->openFromFile(file_name))
	{
		throw std::invalid_argument("Tried to load a music file that wasn't valid!");
		return;
	}
	Musics.emplace(music_name, std::move(music));
}

void Audio::Music_Play(const std::string music_name, float volume, float pitch)
{
	if (!Musics.contains(music_name))
	{
		throw std::invalid_argument("No music of that name has been loaded. Have you loaded the music from file in advance properly?");
		return;
	}
	Musics.find(music_name)->second->setLoop(true);
	Musics.find(music_name)->second->setVolume(volume * 100);
	Musics.find(music_name)->second->setPitch(pitch);
	Musics.find(music_name)->second->play();
}

void Audio::Music_Stop(const std::string music_name)
{
	if (!Musics.contains(music_name))
	{
		throw std::invalid_argument("No music of that name has been loaded. Have you loaded the music from file in advance properly?");
		return;
	}
	Musics.find(music_name)->second->stop();
}

void Audio::Music_Set_Volume(const std::string music_name, float coefficient)
{
	if (!Musics.contains(music_name))
	{
		throw std::invalid_argument("No music of that name has been loaded. Have you loaded the music from file in advance properly?");
		return;
	}
	Musics.find(music_name)->second->setVolume(coefficient);
}

void Audio::Music_Set_Pitch(const std::string music_name, float coefficient)
{
	if (!Musics.contains(music_name))
	{
		throw std::invalid_argument("No music of that name has been loaded. Have you loaded the music from file in advance properly?");
		return;
	}
	Musics.find(music_name)->second->setPitch(coefficient);
}

void Audio::Music_Stop_all_music()
{
	for (auto& music : Musics)
	{
		music.second->stop();
	}
}

void Audio::Music_Adjust_volume_of_all_musics(float coefficient)
{
	for (auto& music : Musics)
	{
		music.second->setVolume(music.second->getVolume() * coefficient);
	}
}



/*
unsigned long long Audio::Music_Load_from_file(const std::string file_name)
{
	// If we have already loaded this music, exit immediately so we don't load a duplicate.
	sf::Music music;
	if (!music.openFromFile(file_name))
	{
		throw std::invalid_argument("Tried to load a music file that wasn't valid!");
		return -1;
	}
	Musics.push_back(music);
	return Musics.size() - 1;
}

void Audio::Music_Play(const int track_index, float volume, float pitch)
{
	Musics[track_index].setLoop(true);
	Musics[track_index].setVolume(volume);
	Musics[track_index].setPitch(pitch);
	Musics[track_index].play();
}

void Audio::Music_Stop(const int track_index)
{
	Musics[track_index].stop();
}

void Audio::Music_Set_Volume(const int track_index, float coefficient)
{
	Musics[track_index].setVolume(coefficient);
}

void Audio::Music_Set_Pitch(const int track_index, float coefficient)
{
	Musics[track_index].setPitch(coefficient);
}
*/

