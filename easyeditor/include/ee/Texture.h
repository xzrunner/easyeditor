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

	virtual const uint8_t* GetPixelData() const { return NULL; }
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetChannels() const = 0;

	virtual void LoadFromFile(const std::string& filepath) {}
	virtual void LoadFromMemory(ImageData* img_data) {}
	virtual void Reload() = 0;

	virtual const ImageData* GetImageData() const { return NULL; }

}; // Texture

}

#endif // _EASYEDITOR_TEXTURE_H_