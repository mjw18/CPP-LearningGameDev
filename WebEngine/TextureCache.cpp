#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace WebEngine {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		GLTexture newTexture;
		auto mit = m_textureMap.find(texturePath);

		//image not in map, needs to be loaded
		if (mit == m_textureMap.end())
		{
			newTexture = ImageLoader::loadPNG(texturePath);

			m_textureMap.insert(make_pair(texturePath, newTexture));

			return newTexture;
		}

		//image is in map get texture
		return mit->second;
	}

}