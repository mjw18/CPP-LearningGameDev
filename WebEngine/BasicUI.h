/*
The script called by the button must have lua state with class previously exposed passed to it

*/
#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Camera2D.h"
#include "ColorRGBA8.h"
#include "InputManager.h"
#include "DebugRenderer.h"
#include "LuaEngine.h"

namespace WebEngine {

	class LuaEngine;

	class Button
	{
	public:
		Button(Camera2D* camera);
		~Button();

		void init(const glm::vec2& position,const glm::vec2& dimensions, const char* buttonText, const char* scriptFile, ColorRGBA8 color = ColorRGBA8(255,255,255,255)); ///>init button
		void close(); ///>clean up buttton

		void update(InputManager& inputManager, LuaEngine& luaEngine); ///> call button action (script) on click, return
		void draw(SpriteBatch& spriteBatch); ///> draw button on screen
		void drawDebug(DebugRenderer& debugRenderer);

		//getters
		const glm::vec2 getPos() const { return m_position; }
		bool getButtonPress() const { return m_buttonPressed; }
		const std::string getFile() const { return m_scriptFile; }

		void registerScreenLua(LuaEngine& LEngine)
		{
			//Register MenuScreen class
			luabind::module(LEngine.getLua())
				[
					luabind::class_<Button >("Button")
				];
			//Register GameplaySceen object in globals
			luabind::globals(LEngine.getLua())["Button"] = this;
		}

	private:
		glm::vec2 m_position;
		glm::vec2 m_dimensions;

		std::string m_scriptFile;
		std::string m_buttonText;

		ColorRGBA8 m_color;

		Camera2D* m_camera;
		SpriteFont* m_spriteFont;///> This sucks, use a resource manager!!!!!!

		bool m_buttonPressed = false;
	};

	class BasicUI
	{
	public:

		BasicUI();
		~BasicUI();

		void init(const char* filePath, Camera2D* camera);///>XML file path from which UI elements are initialized
		void close();

		void update(InputManager& inputManager, LuaEngine& luaEngine);
		void draw(GLSLProgram& textureProgram);
		void drawDebug(DebugRenderer& debugRenderer);

		bool getPressed() const { return buttonPress; }

	private:
		std::vector<Button*> buttonList;
		Camera2D* m_camera;

		SpriteBatch m_spriteBatch;
		bool buttonPress = false;
	};
}


