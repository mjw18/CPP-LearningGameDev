#include "BallRenderer.h"

void BallRenderer::renderBalls(WebEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Lazy init -> init only when needed
	if (mProgram == nullptr)
	{
		mProgram = std::make_unique<WebEngine::GLSLProgram>();
		mProgram->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		mProgram->addAttribute("vertexPosition");
		mProgram->addAttribute("vertexColor");
		mProgram->addAttribute("vertexUV");
		mProgram->linkShaders();
	}
	mProgram->use();

	spriteBatch.begin();

	// Make sure the shader uses texture 0
	GLint textureUniform = mProgram->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	GLint pUniform = mProgram->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Render all the balls
	for (auto& ball : balls){
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);
		spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, ball.color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	mProgram->unuse();
}
