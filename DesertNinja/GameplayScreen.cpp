#include "GameplayScreen.h"


GameplayScreen::GameplayScreen(WebEngine::Window* window, WebEngine::Time* time) : m_Window(window), m_Time(time)
{
}


GameplayScreen::~GameplayScreen()
{
}


int GameplayScreen::getNextScreenIndex() const
{
	return 0;
}

int GameplayScreen::getPreviousScreenIndex() const
{
	return 0;
}

//Called at beginning and end of application
void GameplayScreen::init()
{
	//Initialize shaders
	m_TextureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_TextureProgram.addAttribute("vertexPosition");
	m_TextureProgram.addAttribute("vertexColor");
	m_TextureProgram.addAttribute("vertexUV");
	m_TextureProgram.linkShaders();

	//Init SpriteBatches
	m_DebugRenderer.init();
	m_SpriteBatch.init();

	m_Camera.init(m_Window->getScreenWidth(), m_Window->getScreenHeight());
	m_Camera.setScale(32.0f); //32 pixels per meter, good scale

	testLevel.init();

	m_player.init(testLevel.getWorld(), mGame->InputManager, glm::vec2(12.0f, 3.0f), WebEngine::ColorRGBA8(255, 255, 255, 255));
}

void GameplayScreen::close()
{
	
}

//When screen is switched to
void GameplayScreen::onEntry()
{

}
//When leaving screen
void GameplayScreen::onExit()
{

}

void GameplayScreen::update() 
{
	/*
	WHY HAVE I IGNORED THE TIMESTEP!!!???!!!
	*/

	m_Camera.update();

	//checkInput();	

	m_player.update();
	if (mGame->InputManager.isKeyPressed(SDLK_0))
	{
		changeNext();
	}
	
	testLevel.update();
}

void GameplayScreen::draw() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.2f, 1.0f);

	m_TextureProgram.use();

	//get texture uniform
	GLint textureUniform = m_TextureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0); //Upload uniform data
	glActiveTexture(GL_TEXTURE0);

	//Camera matrix
	glm::mat4 projectionMatrix = m_Camera.getCameraMatrix();
	GLint projectionUniform = m_TextureProgram.getUniformLocation("projectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_SpriteBatch.begin();

	testLevel.draw(m_SpriteBatch);

	m_SpriteBatch.end();

	m_SpriteBatch.renderBatch();

	m_TextureProgram.unuse();

	//Debug Rendering
	if (m_renderDebug)
	{
		//Draw circle around center of screen
		m_DebugRenderer.drawCircle(m_Camera.screenToWorld(glm::vec2(m_Window->getScreenWidth() / 2.0f, m_Window->getScreenHeight() / 2.0f)), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//Draw circe around mouse pisition
		m_DebugRenderer.drawCircle(m_Camera.screenToWorld(glm::vec2(mGame->InputManager.getMouseCoords())), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//mDebugRenderer.drawLine(mCamera.screenToWorld(glm::vec2(mWindow->getScreenWidth() / 2.0f, mWindow->getScreenHeight() / 2.0f)), 4.0f, WebEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);
		//Draw camera debug
		m_Camera.drawDebug(m_DebugRenderer);
		
		testLevel.drawDebug(m_DebugRenderer);
		m_player.debugDraw(m_DebugRenderer);

		m_DebugRenderer.end();
		m_DebugRenderer.render(projectionMatrix, 2.0f);
	}
}

//I externalized this, maybe need it back? limit number of events/update? we'll see
void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		mGame->onSDLEvent(evnt);
	}
}
