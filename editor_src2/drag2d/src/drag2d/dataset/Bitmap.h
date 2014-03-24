#pragma once

#include "common/ResourcesMgr.h"
#include "common/Object.h"

#include <wx/wx.h>

namespace d2d
{
	class Bitmap;
	typedef ResourcesMgr<Bitmap> BitmapMgr;

	class Bitmap : public Object
	{
	public:
		Bitmap();
		Bitmap(wxBitmap* bitmap);
		virtual ~Bitmap();

		bool loadFromFile(const wxString& filepath);

		const wxBitmap* getBitmap() const {
			return m_bitmap;
		}

	private:
		static void getImage(const wxString& filepath, wxImage& image);

		static wxBitmap* getBitmap(const wxImage& image);

		static float computeScale(float width);

	private:
		wxString m_filename;

		wxBitmap* m_bitmap;

		float m_scale;

	}; // Bitmap
}

