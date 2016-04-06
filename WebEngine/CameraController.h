#pragma once
#include <glm/glm.hpp>

namespace WebEngine{
	//Set boundary at which player pushes camera to move
	struct XShiftAxis
	{
		float width;
		float y;
	};
	struct YShiftAxis
	{
		float height;
		float x;
	};
	//Create object that pulls in camera
	/* x and y are attractor center
	Radius gives radius of attractor, when camera center enters radius, snap to
	Strength gives weighted avergae
	*/
	struct Attractor
	{
		float x;
		float y;
		float radius;
		float strength;
	};

	class Time;
	class Camera2D;

	class CameraController
	{
	public:
		CameraController();
		~CameraController();

		void init(Camera2D* camera, Time* time);

		void smoothCameraMove(const glm::vec2& startPos, const glm::vec2& targetPos, float time);
		void update();

		glm::vec2 camPos;
	private:
		float lerp(float a, float b, float t) { return a + t * (b - a); }

		Camera2D* m_Camera = nullptr;
		Time* m_Time = nullptr;

	};
}


