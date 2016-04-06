#include "Particle2DManager.h"

#include "SpriteBatch.h"
#include "ParticleBatch2D.h"

namespace WebEngine{

	Particle2DManager::Particle2DManager()
	{
		//Empty
	}


	Particle2DManager::~Particle2DManager()
	{
		for (auto& p : mBatches)
		{
			delete p;
		}
	}

	void Particle2DManager::addParticleBatch(ParticleBatch2D* particleBatch)
	{
		mBatches.push_back(particleBatch);
	}

	void Particle2DManager::update(float deltaTime)
	{
		for (auto& p : mBatches)
		{
			p->update(deltaTime);
		}
	}

	void Particle2DManager::draw(SpriteBatch* spriteBatch)
	{
		//For each particle batch, draw it
		for (auto& p : mBatches)
		{
			spriteBatch->begin();	
			p->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}
	}

}