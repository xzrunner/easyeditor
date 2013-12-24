#pragma once

#include "common/ResourcesMgr.h"
#include "common/Rect.h"

#include <wx/wx.h>

namespace d2d
{
	class Image;
	typedef ResourcesMgr<Image> ImageMgr;

	class Image
	{
	public:
		Image();
		~Image();
		
		bool loadFromFile(const wxString& filepath);
		void reload();

		const wxString& filepath() const { return m_filepath; }

		unsigned int textureID() const { return m_textureID; }
		int width() const { return m_width; }
		int height() const { return m_height; }

		void draw() const;

		const Rect& getRegion() const { return m_region; }

	private:
		void removeTransparentBorder();
		bool isTransparent(unsigned char* pixels, int x, int y, int channels);

	private:
		wxString m_filepath;

		unsigned int m_textureID;
		int m_width, m_height;
		Rect m_region;
		int m_format;

		unsigned char* m_pixels;

	}; // Image
}

