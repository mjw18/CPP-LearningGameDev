#include "BasicUI.h"
#include "XMLParser.h"
#include "LuaEngine.h"

namespace WebEngine
{
	Button::Button(Camera2D* camera) : m_camera(camera)
	{
	}


	Button::~Button()
	{
		delete m_spriteFont;
	}

	void Button::init(const glm::vec2& position, const glm::vec2& dimensions, const char* buttonText, const char* scriptFile, ColorRGBA8 color /*= WHITE*/)
	{
		m_position = position;
		m_dimensions = dimensions;
		m_buttonText = buttonText;
		m_scriptFile = scriptFile;
		m_color = color;

		m_spriteFont = new SpriteFont("Fonts/Hachicro.ttf", 24);///> This sucks, use a resource manager!!!!!!
	}

	void Button::close()
	{
		delete m_spriteFont;
	}

	void Button::update(InputManager& inputManager, LuaEngine& luaEngine)
	{
		glm::vec2 mouseCoords = m_camera->screenToWorld(inputManager.getMouseCoords());

		if (mouseCoords.x >= (m_position.x) && mouseCoords.x <= (m_position.x + m_dimensions.x))
		{
			if (mouseCoords.y >= m_position.y && mouseCoords.y <= (m_position.y + m_dimensions.y))
			{
				m_color = ColorRGBA8(255, 0, 0, 255);
				if (inputManager.isKeyPressed(SDL_BUTTON_LEFT))
				{
					m_buttonPressed = true;

					//Allow for string input (file might be excessive in some cases
					luaEngine.executeFile(m_scriptFile.c_str());

					return;
				}
			}
			else
			{
				m_color = ColorRGBA8(0, 255, 0, 255);
			}
		}
		else
		{
			m_color = ColorRGBA8(255, 255, 255, 255);
		}
		m_buttonPressed = false;
	}

	void Button::draw(SpriteBatch& spriteBatch)
	{
		glm::vec2 textMeasure = m_spriteFont->measure(m_buttonText.c_str());
		
		//Render text
		m_spriteFont->draw(spriteBatch, m_buttonText.c_str(), m_position,
			glm::vec2((m_dimensions.x / (float)textMeasure.x), (m_dimensions.y / (float)textMeasure.y)), 0.0f, m_color);
	}

	void Button::drawDebug(DebugRenderer& debugRenderer)
	{
		glm::vec4 destRect(m_position, m_dimensions);

		debugRenderer.drawBox(destRect, m_color, 0.0);
	}

	BasicUI::BasicUI()
	{

	}

	BasicUI::~BasicUI()
	{

	}

	void BasicUI::init(const char* filePath, Camera2D* camera)
	{
		m_camera = camera;
		m_spriteBatch.init();

		//Declare document tree
		pugi::xml_document doc;
		XMLParser::loadXML(filePath, doc);

		pugi::xml_node UIAtlas = doc.first_child();

		for (pugi::xml_node buttonNode = UIAtlas.first_child(); buttonNode; buttonNode = buttonNode.next_sibling())
		{
			//Allocate button on heap
			Button* button = new Button(camera);

			//Parse XML data for button
			float x = buttonNode.attribute("x").as_float();
			float y = buttonNode.attribute("y").as_float();
			float w = buttonNode.attribute("width").as_float();
			float h = buttonNode.attribute("height").as_float();
			std::string buttonText = buttonNode.attribute("text").as_string();
			std::string scriptFile = buttonNode.attribute("script").as_string();

			//Initialize button 
			button->init(glm::vec2(x, y), glm::vec2(w, h), buttonText.c_str(), scriptFile.c_str() );
			//Add button to list
			buttonList.push_back(button);
		}

	}

	void BasicUI::close()
	{
		for (size_t i = 0; i < buttonList.size(); ++i)
		{
			buttonList[i]->close();
			delete buttonList[i];
		}
	}

	void BasicUI::update(InputManager& inputManager, LuaEngine& luaEngine)
	{
		for (size_t i = 0; i < buttonList.size(); ++i)
		{
			buttonList[i]->update(inputManager, luaEngine);
			//Check for button press
			if (buttonList[i]->getButtonPress())
			{
				printf("%s", buttonList[i]->getFile().c_str());
				buttonPress = true;
				return;
			}
		}
		buttonPress = false;
	}

	void BasicUI::draw(GLSLProgram& textureProgram)
	{
		glm::mat4 projMatrix = m_camera->getCameraMatrix();
		GLint projectionUniform = textureProgram.getUniformLocation("projectionMatrix");
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projMatrix[0][0]);

		m_spriteBatch.begin();

		for (size_t i = 0; i < buttonList.size(); ++i)
		{
			buttonList[i]->draw(m_spriteBatch);
		}

		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
	}

	void BasicUI::drawDebug(DebugRenderer& debugRenderer)
	{
		for (size_t i = 0; i < buttonList.size(); ++i)
		{
			buttonList[i]->drawDebug(debugRenderer);
		}
	}
}
