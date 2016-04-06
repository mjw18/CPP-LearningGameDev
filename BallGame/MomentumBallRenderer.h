#pragma once

#include "BallRenderer.h"

class MomentumBallRenderer : public BallRenderer
{
public:
	virtual void renderBalls(WebEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, 
							 const glm::mat4& projectionMatrix) override;
};

