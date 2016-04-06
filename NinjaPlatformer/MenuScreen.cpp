#include "MenuScreen.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <WebEngine/IMainGame.h>
#include <WebEngine/GLTexture.h>
#include <WebEngine/ResourceManager.h>
#include <WebEngine/BasicUI.h>
#include <iostream>

MenuScreen::MenuScreen(WebEngine::Window* window, WebEngine::Time* time) : m_Window(window), m_Time(time)
{
}

MenuScreen::~MenuScreen()
{
}

int MenuScreen::getNextScreenIndex() const
{
	return 1;
}

int MenuScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

//Called at beginning and end of application
void MenuScreen::init()
{
	m_debugRenderer.init();
	mSpriteBatch.init();

	//Initialize shaders
	//Compile texture shader
	mTextureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	mTextureProgram.addAttribute("vertexPosition");
	mTextureProgram.addAttribute("vertexColor");
	mTextureProgram.addAttribute("vertexUV");
	mTextureProgram.linkShaders();

	m_Camera.init(m_Window->getScreenWidth(), m_Window->getScreenHeight());
	m_Camera.setScale(32.0f); //32 pixels per meter, good scale

	m_UI.init("XMLData/titleUI.xml", &m_Camera);
}
void MenuScreen::close()
{
	mTextureProgram.close();
	m_debugRenderer.close();
	for (size_t i = 0; i < buttonList.size(); ++i)
	{
		buttonList[i]->close();
	}
}

//When screen is switched to
void MenuScreen::onEntry()
{
	
}
//When leaving screen
void MenuScreen::onExit()
{
	
}

void MenuScreen::update()
{
	//checkInput();
	m_Camera.update();

	m_UI.update(mGame->InputManager, mGame->LEngine);

	if (m_UI.getPressed())
	{
		std::cout << "\n***press***\n";
	}
}

void MenuScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.4f, 1.0f);

	mTextureProgram.use();

	//get texture uniform
	GLint textureUniform = mTextureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0); //Upload uniform data
	glActiveTexture(GL_TEXTURE0);

	//Camera matrix
	glm::mat4 projectionMatrix = m_Camera.getCameraMatrix();
	GLint projectionUniform = mTextureProgram.getUniformLocation("projectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	mSpriteBatch.begin();

	m_UI.draw(mSpriteBatch);

	mSpriteBatch.end();

	mSpriteBatch.renderBatch();

	mTextureProgram.unuse();

	//Debug Rendering
	if (m_renderDebug)
	{
		//Draw circle around center of screen
		m_debugRenderer.drawCircle(m_Camera.screenToWorld(glm::vec2(m_Window->getScreenWidth() / 2.0f, m_Window->getScreenHeight() / 2.0f)), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//Draw circe around mouse pisition
		m_debugRenderer.drawCircle(m_Camera.screenToWorld(glm::vec2(mGame->InputManager.getMouseCoords())), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//mDebugRenderer.drawLine(mCamera.screenToWorld(glm::vec2(mWindow->getScreenWidth() / 2.0f, mWindow->getScreenHeight() / 2.0f)), 4.0f, WebEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);
		m_UI.drawDebug(m_debugRenderer);
		//Draw camera debug
		m_Camera.drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}
}

void MenuScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		mGame->onSDLEvent(evnt);
	}
}