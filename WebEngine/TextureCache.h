#pragma once
///>ToDo: Memory Leak???!!??
#include <map>
#include "GLTexture.h"

namespace WebEngine {

	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string);

	private:
		std::map<std::string, GLTexture> m_textureMap;
	};

}