#pragma once

#include <WebEngine/Camera2D.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/InputManager.h>
#include <WebEngine/Window.h>
#include <WebEngine/GLSLProgram.h>
#include <WebEngine/Timing.h>
#include <WebEngine/SpriteFont.h>
#include <memory>

#include "Grid.h"
#include "BallController.h"
#include "BallRenderer.h"
#include "MomentumBallRenderer.h"

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame {
public:
	~MainGame();
    void run();

private:
    void init();
	void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

	int mCurrentRenderer = 0;
    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls
	std::unique_ptr<Grid> mGrid = nullptr;// Grid for spatial partitioning
	std::vector<BallRenderer*> mBallRenderers;

    BallController m_ballController; ///< Controls balls
    BallRenderer m_ballRenderer; ///< Renders balls

    WebEngine::Window m_window; ///< The main window
    WebEngine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<WebEngine::SpriteFont> m_spriteFont; ///< For font rendering
    WebEngine::Camera2D m_camera; ///< Renders the scene
    WebEngine::InputManager m_inputManager; ///< Handles input
    WebEngine::GLSLProgram m_textureProgram; ///< Shader for textures]

	WebEngine::Time m_Time;
    WebEngine::FPSLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

