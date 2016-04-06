#include "Timing.h"
#include <SDL/SDL.h>
#include <algorithm>

namespace WebEngine{

	FPSLimiter::FPSLimiter()
	{

	}

	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS)
	{
		_maxFPS = maxFPS;
	}

	void FPSLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}

	//end will return the FPS
	float FPSLimiter::end()
	{
		//calculate fps before limiting
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		//Limit FPS
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay((Uint32)(1000.f / _maxFPS) - frameTicks);
		}

		return _FPS;
	}

	//calculate FPS average
	void FPSLimiter::calculateFPS()
	{
		//g variable only aaccessible in calculateFPS
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float previousTicks = SDL_GetTicks();

		float currentTicks;
		currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - previousTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		previousTicks = currentTicks;

		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float averageFrameTimes = 0;
		for (int i = 0; i < count; i++)
		{
			averageFrameTimes += frameTimes[i];
		}
		averageFrameTimes /= count;

		if (averageFrameTimes != 0)
		{
			_FPS = 1000.0f / averageFrameTimes;
		}
		else
		{
			_FPS = 0.0f;
		}
	}

	Time::Time()
	{
	}

	void Time::init()
	{
		m_startTime = SDL_GetTicks();
		m_CurrentTicks = m_startTime;
	}

	float Time::gameTime()
	{
		m_CurrentTicks = SDL_GetTicks();
		Uint32 time = m_CurrentTicks - m_startTime;

		return (float)time / MS_PER_SECOND;
	}

	uint32_t Time::getTicks() 
	{ 
		return SDL_GetTicks(); 
	}

	void Time::calculateDeltaTime()
	{
		// Calculate the frameTime in milliseconds
		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - m_CurrentTicks;
		m_CurrentTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
		// Get the total delta time
		m_totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;
		// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
		m_deltaTime = std::min(m_totalDeltaTime, MAX_DELTA_TIME);
		// Update all physics here and pass in deltaTime
	
	}
}