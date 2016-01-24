#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"
#include <SDL\SDL_image.h>

GLTexture ImageLoader::loadPNG(CStr filePath) {
    //Create a GLTexture and initialize all its fields to 0
    GLTexture tex = {};
    
    //This is the input data to decodePNG, which we load from a file
    std::vector<unsigned char> in;
    //This is the output data from decodePNG, which is the pixel data for our texture
    std::vector<unsigned char> out;
	
    //Read in the image file contents into a buffer
    if (IOManager::readFileToBuffer(filePath, in) == false) {
        fatalError("Failed to load PNG file to buffer!");
    }

	unsigned long w, h;

    //Decode the .png format into an array of pixels
    int errorCode = decodePNG(out, w, h, &(in[0]), in.size());
    if (errorCode != 0) {
        fatalError("decodePNG failed with error: " + std::to_string(errorCode));
    }

	tex.Width = w;
	tex.Height = h;

    //Generate the openGL texture object
    glGenTextures(1, &(tex.ID));

    //Bind the texture object
    glBindTexture(GL_TEXTURE_2D, tex.ID);

    //Upload the pixels to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.Width, tex.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    //Set some texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //Generate the mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    //Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
	
    //Return a copy of the texture data
    return tex;
}

GLTexture ImageLoader::loadImage(CStr file)
{
	GLTexture tex = { 0 };

	SDL_Surface* surface = IMG_Load(file.c_str());

	if (!surface)
	{
		fatalError("Error loding image: " + file);
	}

	tex.Width = surface->w;
	tex.Height = surface->h;

	int mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4)
		mode = GL_RGBA;

	//Generate the openGL texture object
	glGenTextures(1, &tex.ID);

	//Bind the texture object
	glBindTexture(GL_TEXTURE_2D, tex.ID);

	//Upload the pixels to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	//Set some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}
