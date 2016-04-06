#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WebEngine{

	class DebugRenderer;

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		//transform 2D vector from screen to world coordinates
		glm::vec2 screenToWorld(glm::vec2 screenCoords);

		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

		void drawDebug(DebugRenderer& debugRenderer);

		//setters
		void setPosition(const glm::vec2& newPosition){ mPosition = newPosition; mNeedsUpdate = true; }
		void setScale(float newScale){ mScale = newScale; mNeedsUpdate = true; }

		//getters
		glm::vec2 getPosition(){ return mPosition; }
		float getScale(){ return mScale; }
		glm::mat4 getCameraMatrix(){ return mCameraMatrix; }

	private:
		int mScreenWidth;
		int mScreenHeight;

		bool mNeedsUpdate;
		//zooming factor
		float mScale;

		glm::vec2 mPosition;
		glm::mat4 mCameraMatrix;
		glm::mat4 mOrthoMatrix;
	};

}