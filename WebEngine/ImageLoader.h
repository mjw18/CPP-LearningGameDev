#pragma once

#include "GLTexture.h"
#include <string>

namespace WebEngine {

	class ImageLoader
	{
	public:

		static GLTexture loadPNG(std::string filePath);

	};

}