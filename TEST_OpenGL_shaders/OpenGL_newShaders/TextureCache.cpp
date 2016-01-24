#include "TextureCache.h"
#include "ImageLoader.h"
#include "OpenGLUtility.h"
#include <algorithm>

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}


GLTexture TextureCache::getTexture(std::string texturePath) {

    //lookup the texture and see if its in the map
	std::transform(texturePath.begin(), texturePath.end(), texturePath.begin(), toupper);

    auto mit = _textureMap.find(texturePath);
    
    //check if its not in the map
    if (mit == _textureMap.end()) {
        //Load the texture
        GLTexture newTexture = ImageLoader::loadImage(texturePath);
        
        //Insert it into the map
        _textureMap.insert(make_pair(texturePath, newTexture));

        std::cout << "Loaded Texture!\n";
        return newTexture;
    }
    std::cout << "Used Cached Texture!\n";
    return mit->second;
}