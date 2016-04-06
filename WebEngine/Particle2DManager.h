#pragma once

#include <vector>

namespace WebEngine{
	//Declare inside namespace or use ::
	class ParticleBatch2D;
	class SpriteBatch;

	class Particle2DManager
	{
	public:
		Particle2DManager();
		~Particle2DManager();

		//after adding a particle batch, the ParticleBtach2D becomes responsible
		//for deallocation
		void addParticleBatch(ParticleBatch2D* particleBatch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch2D*> mBatches;
	};

}