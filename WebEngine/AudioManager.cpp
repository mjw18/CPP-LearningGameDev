#include "AudioManager.h"
#include "Err.h"

namespace WebEngine{

	void SoundEffect::play(int loops /* = 0 */)
	{
		if (Mix_PlayChannel(-1, mChunk, loops) == -1)
		{
			//FIXXXXX loop through available and find oldest use that
			if (Mix_PlayChannel(0, mChunk, loops) == -1)
			{
				fatalError("Mix Play Channel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops /* = 1 */)
	{
		if (Mix_PlayMusic( mMusic, loops) == -1)
		{
			fatalError("Mix Play Music error: " + std::string(Mix_GetError()));
		}
	}

	void Music::fadeInMusic(int fadeTime, int loops /* = -1 */)
	{
		if (Mix_FadeInMusic(mMusic, loops, fadeTime) == -1)
		{
			fatalError("Mix Fade in Music error: " + std::string(Mix_GetError()));
		}
	}

	void Music::fadeOutMusic(int ms)
	{
		Mix_FadeOutMusic(ms);
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}
	void Music::resume()
	{
		Mix_ResumeMusic();
	}
	void Music::stop()
	{
		Mix_HaltMusic();
	}

	AudioManager::AudioManager()
	{
		//Empty
	}


	AudioManager::~AudioManager()
	{
		close();
	}

	void AudioManager::init()
	{
		//Parameter can be a bitwise combo of MIX_INIT_FAC, MIX_INIT_MOD, MP3, OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			WebEngine::fatalError("SDL_Mixer failed to initialize. Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			WebEngine::fatalError("SDL_Mixer failed to initialize. Mix_Init error: " + std::string(Mix_GetError()));
		}

		//Check at close
		mIsInitialized = true;
	}

	void AudioManager::close()
	{
		if (mIsInitialized)
		{
			mIsInitialized = false;

			for (auto& it : mEffectMap)
			{
				Mix_FreeChunk(it.second);
			}

			for (auto& it : mMusicMap)
			{
				Mix_FreeMusic(it.second);
			}

			mEffectMap.clear();
			mMusicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioManager::loadSoundEffect(const std::string& filePath)
	{
		SoundEffect effect;

		auto it = mEffectMap.find(filePath);

		if (it == mEffectMap.end())
		{
			//File not found. Load audio file
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

			//Check for errors
			if (chunk == nullptr)
			{
				fatalError("Mix_LoadWav error: " + std::string(Mix_GetError()));
			}

			effect.mChunk = chunk;
			mEffectMap.insert(make_pair(filePath, chunk));
		}
		else
		{
			effect.mChunk = it->second;
		}
		//Return effect obj
		return effect;
	}

	Music AudioManager::loadMusic(const std::string& filePath)
	{
		Music music;

		auto it = mMusicMap.find(filePath);

		if (it == mMusicMap.end())
		{
			//File not found. Load audio file
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());

			//Check for errors
			if (mixMusic == nullptr)
			{
				fatalError("Mix_LoadWav error: " + std::string(Mix_GetError()));
			}

			music.mMusic = mixMusic;
			mMusicMap.insert(make_pair(filePath, mixMusic));
		}
		else
		{
			music.mMusic = it->second;
		}
		//Return music obj
		return music;
	}
}