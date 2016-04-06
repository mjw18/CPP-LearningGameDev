#pragma once

#include <string>
#include <map>
#include <SDL/SDL_mixer.h>

namespace WebEngine{

	class SoundEffect
	{
	public:
		friend class AudioManager;

		//Loops is how many times you want to play the effect
		//loop = 0 play once
		void play(int loops = 0);

	private:

		Mix_Chunk* mChunk = nullptr;

	};

	class Music
	{
	public:
		friend class AudioManager;

		//P loops: if loop == -1, play forever. 
		//Otherwise play loops times
		void play(int loops = 1);

		//Fade in then loop loops times
		//loops = -1 plays forever
		//fade Time in milliseconds
		void fadeInMusic(int fadeTime, int loops = 1);

		//Fade out music over ms milliseconds from when called
		void fadeOutMusic(int ms);

		//volume = -1 does not set volume
		void setVolume(int volume = -1);

		static void pause();
		static void resume();
		static void stop();
		
	private:
		Mix_Music* mMusic = nullptr; 
	};

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		void init();
		void close();

		//Caches sound files
		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:

		//Sound data caches. Effects and Music are different
		std::map<std::string, Mix_Chunk*> mEffectMap;
		std::map<std::string, Mix_Music*> mMusicMap;

		//Member variables
		bool mIsInitialized = false; //Has mix been initialized?
	};

}