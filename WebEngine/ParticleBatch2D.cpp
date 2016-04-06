#include "ParticleBatch2D.h"

namespace WebEngine {

	ParticleBatch2D::ParticleBatch2D()
	{
		//Empty
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] mParticles;
	}

	//Max number this batch can handle
	void ParticleBatch2D::init(int maxParticles, 
							   float decayRate, 
							   GLTexture texture, 
							   std::function<void(Particle2D&, float)> updateFunc /* = defaultParticleUpdate */)
	{
		mMaxParticles = maxParticles;
		mParticles = new Particle2D[maxParticles];
		mDecayRate = decayRate;
		mTexture = texture;
		mUpdateFunc = updateFunc;

	}
	//Activate a preallocated particle
	void ParticleBatch2D::addParticle(const glm::vec2& position,
									  const glm::vec2& velocity,
									  const ColorRGBA8 color,
									  float width)
	{
		int particleIndex = findFreeParticle();

		//Reduce size of code by assigning reference to array member
		auto& p = mParticles[particleIndex];

		p.lifeTime = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;

	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < mMaxParticles; i++)
		{
			if (mParticles[i].lifeTime > 0.0f)
			{
				mUpdateFunc(mParticles[i], deltaTime);
				mParticles[i].lifeTime -= mDecayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		for (int i = 0; i < mMaxParticles; i++)
		{
			auto& p = mParticles[i]; //get reference, reduce code
			if (mParticles[i].lifeTime >= 0.0f)
			{
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);// destination rectangle
				spriteBatch->draw(destRect, uvRect, mTexture.id, 0.0f, p.color);
			}
		}
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = mLastFreeParticle; i < mMaxParticles; i++)
		{
			if (mParticles[i].lifeTime <= 0.0f)
			{
				mLastFreeParticle = i;
				return i;
			}
		}

		//end of array is full search beginning
		for (int i = 0; i < mMaxParticles; i++)
		{
			if (mParticles[i].lifeTime <= 0.0f)
			{
				mLastFreeParticle = i;
				return i;
			}
		}
		//No particles are free, overwite first one
		return 0;
	}


}