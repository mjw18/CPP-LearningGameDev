#include "Camera2D.h"
#include "DebugRenderer.h"

namespace WebEngine{

	Camera2D::Camera2D() :
		mPosition(0.0f, 0.0f),
		//identity matrix by default
		mCameraMatrix(1.0f),
		mOrthoMatrix(1.0f),
		mNeedsUpdate(true), 
		mScreenWidth(500),
		mScreenHeight(500),
		mScale(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		mOrthoMatrix = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
	}

	void Camera2D::update()
	{
		if (mNeedsUpdate)
		{	

			//Translate matrix by negative of position to imitate camera moving
			glm::vec3 translationVector(-mPosition.x + mScreenWidth / 2.0f , -mPosition.y + mScreenHeight / 2.0f , 0.0f);
			mCameraMatrix = glm::translate(mOrthoMatrix, translationVector);

			//Camera matrix scale, whole numbers scale closer
			glm::vec3 scaleVector(mScale, mScale, 0.0f);
			mCameraMatrix = glm::scale(glm::mat4(1.0f), scaleVector) * mCameraMatrix;

			mNeedsUpdate = false;
		}
	}

	glm::vec2 Camera2D::screenToWorld(glm::vec2 screenCoords)
	{	
		
		//invert y direction
		screenCoords.y = mScreenHeight - screenCoords.y;
		//0 is at center
		screenCoords -= glm::vec2(mScreenWidth / 2.0f, mScreenHeight / 2.0f);
		//scale coordinates
		screenCoords /= mScale;
		//translate camera
		screenCoords += mPosition;

		return screenCoords;
	}

	//Simple AABB collision test to not draw entites outside of screen
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions)
	{
		glm::vec2 scaledScreenDimensions = glm::vec2(mScreenWidth, mScreenHeight) / ( mScale );
		//X and Y minimum approach distances
		const float MIN_DISTANCEX = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCEY = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		//Ceneter position of parameters
		glm::vec2 centerPosition = position + dimensions / 2.0f;

		//center position of camera
		glm::vec2 cameraCeneterPosition = mPosition;

		//Vector from object to camera center
		glm::vec2 distVec = centerPosition - cameraCeneterPosition;

		float xDepth = MIN_DISTANCEX - abs(distVec.x);
		float yDepth = MIN_DISTANCEY - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0)
		{
			//There was a collision
			return true;
		}
		return false;
	}
	
	void Camera2D::drawDebug(DebugRenderer& debugRenderer)
	{
		WebEngine::ColorRGBA8 debugColor(255, 255, 255, 255);

		debugRenderer.drawLine(mPosition, 2, debugColor, 0);
		debugRenderer.drawLine(mPosition, 2, debugColor, 1.5709633);
	}

}