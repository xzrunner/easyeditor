#pragma once

#include "IFileAdapter.h"

#include <wx/wx.h>

namespace d2d
{
	class CircleFileAdapter : public IFileAdapter
	{
	public:
		virtual void load(const char* filename);

	public:
		wxString m_imgPath;
		int m_width, m_height;

	}; // CircleFileAdapter
}

