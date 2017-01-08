#ifndef _EASYEDITOR_TEXTURE_H_
#define _EASYEDITOR_TEXTURE_H_

#include <string>

#include <stdint.h>

namespace ee
{

class ImageData;

class Texture
{
public:
	virtual ~Texture() {}

	virtual unsigned int GetTexID() const = 0;

	virtual std::string GetFilepath() const { return ""; }

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetFormat() const = 0;

	virtual void LoadFromMemory(const uint8_t* pixels, int w, int h, int fmt) {}

}; // Texture

}

#endif // _EASYEDITOR_TEXTURE_H_