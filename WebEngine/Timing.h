#pragma once
#include <stdint.h>

namespace WebEngine
{
	
	class FPSLimiter {
	public:
		FPSLimiter();

		void init(float maxFPS);

		void begin();

		void setMaxFPS(float maxFPS);

		//end will return the FPS
		float end();

	private:
		void calculateFPS();

		float _FPS;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;
	};

	class Time
	{
	public:
		Time();

		void init();

		float gameTime();//Time since beginning of game
		
		void calculateDeltaTime();//Pass in the ticks at the beginning of the frame

		uint32_t getTicks(); //Get current number of ticks
		float getDeltaTime() const { return m_deltaTime; } 

		const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
		const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
		const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
		const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
		const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

	private:
		uint32_t m_startTime = 0; //Ticks at beginning of game
		uint32_t m_CurrentTicks = 0;
		float m_totalDeltaTime = 0;
		float m_deltaTime = 0;

	};

}