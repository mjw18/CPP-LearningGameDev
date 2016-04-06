#pragma once

#include <WebEngine/SpriteBatch.h>
#include <WebEngine/GLSLProgram.h>
#include <vector>
#include <memory>
#include "Ball.h"

class BallRenderer {
public:
	virtual void renderBalls(WebEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, 
							 const glm::mat4& projectionMatrix);

protected:
	std::unique_ptr<WebEngine::GLSLProgram> mProgram = nullptr;

};
