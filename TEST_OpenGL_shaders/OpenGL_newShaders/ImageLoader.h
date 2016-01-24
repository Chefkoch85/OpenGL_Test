#pragma once
#include "GLTexture.h"

#include "OpenGLUtility.h"

//Loads images into GLTextures
class ImageLoader
{
public:
    static GLTexture loadPNG(CStr file);
	static GLTexture loadImage(CStr file);
};

