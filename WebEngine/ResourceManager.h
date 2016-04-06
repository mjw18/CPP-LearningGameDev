#pragma once

#include "TextureCache.h"
#include <string>

namespace WebEngine {

	class ResourceManager
	{
	public:
		//texture getter
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache m_textureCache;
	};

}