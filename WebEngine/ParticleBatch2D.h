#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace WebEngine {

	//Particle class
	class Particle2D{
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		WebEngine::ColorRGBA8 color;
		float width = 0.0f; //square particles
		float lifeTime = 0.0f; //Don't want particles to exist forever
	};

	//Default particle update function. Don't use member functions
	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime)
	{
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public://per batch of particle
	
		ParticleBatch2D();
		~ParticleBatch2D();

		//Max number this batch can handle
		void init(int maxParticles, 
				  float decayRate, 
				  GLTexture texture,
				  std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);

		//Activate a preallocated particle
		void addParticle(const glm::vec2& position, 
						 const glm::vec2& velocity, 
						 const ColorRGBA8 color,
						 float width) ;

		void update(float deltaTime);
		
		void draw(SpriteBatch* spriteBatch);

	private:
		int findFreeParticle(); //returns index of last inactive particle

		std::function<void(Particle2D&, float)> mUpdateFunc;
		float mDecayRate = 0.1f;
		Particle2D* mParticles = nullptr;
		int mMaxParticles = 0;
		int mLastFreeParticle = 0; //marker for last particle, reduces time for linear search
		GLTexture mTexture; //particle texture
	};

}