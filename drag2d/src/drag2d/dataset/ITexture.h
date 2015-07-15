#ifndef _DRAG2D_ITEXTURE_H_
#define _DRAG2D_ITEXTURE_H_

typedef unsigned char uint8_t;

#include <string>

namespace d2d
{

class ImageData;

class ITexture
{
public:
	virtual ~ITexture() {}

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

}; // ITexture

}

#endif // _DRAG2D_ITEXTURE_H_