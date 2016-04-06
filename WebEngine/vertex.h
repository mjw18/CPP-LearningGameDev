#pragma once

#include <GL/glew.h>
#include "ColorRGBA8.h"

namespace WebEngine {

	//two since 2D
	struct Position{
		float x;
		float y;
		float z;
	};

	struct UV {
		float u;
		float v;
	};

	//use composition (struct of structs)
	struct Vertex {
		//4 bytes for color
		ColorRGBA8 color;

		Position position;

		//UV texture coordinates
		UV uv;

		//makes setting easier
		void setPosition(float x, float y, float z = 0)
		{
			position.x = x;
			position.y = y;
			position.z = z;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};

}