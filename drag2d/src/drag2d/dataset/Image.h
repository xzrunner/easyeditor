#pragma once

#include "common/ResourcesMgr.h"
#include "common/Rect.h"
#include "common/Object.h"

#include <wx/wx.h>

typedef unsigned char uint8_t;

namespace d2d
{
	class Image;
	typedef ResourcesMgr<Image> ImageMgr;

	class Matrix;
	class ISprite;

	class Image : public Object
	{
	public:
		Image();
		Image(const uint8_t* pixel, int width, int height);
		~Image();
		
		bool loadFromFile(const wxString& filepath);
		void reload();

		const wxString& filepath() const { return m_filepath; }

		unsigned int textureID() const { return m_textureID; }
		int channels() const { return m_channels; }

		int clipWidth() const { return m_region.xLength(); }
		int clipHeight() const { return m_region.yLength(); }
		int originWidth() const { return m_width; }
		int originHeight() const { return m_height; }

		void draw(const Matrix& mt, const Rect& r, const ISprite* spr) const;

		Rect getRegion() const { return m_region; }

		const unsigned char* getPixelData() const { return m_pixels; }

	private:
		wxString m_filepath;

		unsigned int m_textureID;
		int m_channels;

		int m_width, m_height;
		Rect m_region;

		const uint8_t* m_pixels;

	}; // Image
}

