#ifndef _EASYEDITOR_IMAGE_DATA_H_
#define _EASYEDITOR_IMAGE_DATA_H_

#include "ResourcesMgr.h"

#include <CU_RefCountObj.h>

#include <stdint.h>

namespace ee
{

class ImageData;
typedef ResourcesMgr<ImageData> ImageDataMgr;

class ImageData : public cu::RefCountObj
{
public:
	ImageData();
	ImageData(const uint8_t* pixels, int w, int h, int c);
	virtual ~ImageData();

	bool LoadFromFile(const std::string& filepath);

	const std::string& GetFilepath() const { return m_filepath; }

	const uint8_t* GetPixelData() const { return m_pixels; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	int GetChannels() const { return m_channels; }
	int GetFormat() const { return m_format; }

	void SetContent(const uint8_t* pixels, int width, int height);

private:
	std::string m_filepath;

	const uint8_t* m_pixels;
	int m_width, m_height;
	int m_channels;
	int m_format;

}; // ImageData

}

#endif // _EASYEDITOR_IMAGE_DATA_H_