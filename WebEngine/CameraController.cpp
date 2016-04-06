#include "CameraController.h"
#include "Camera2D.h"
#include "Timing.h"
#include <iostream>
namespace WebEngine{
	CameraController::CameraController()
	{
	}


	CameraController::~CameraController()
	{
	}

	void CameraController::init(Camera2D* camera, Time* time)
	{
		m_Camera = camera;
		m_Time = time;
	}

	void CameraController::smoothCameraMove(const glm::vec2& startPos, const glm::vec2& targetPos, float time)
	{
		glm::vec2 lerpPos;
		float i = 0;

		float rate = 1.0f / time;

		while (i < 1.0f)
		{
			i += rate * m_Time->getDeltaTime();
			lerpPos.x = lerp(startPos.x, targetPos.x, i);
			lerpPos.y = lerp(startPos.y, targetPos.y, i);

			//std::cout << i << " " << m_Time->getDeltaTime();
			m_Camera->setPosition(lerpPos);
		}
	}

	void CameraController::update()
	{
		//m_Camera->update();
	}
}

