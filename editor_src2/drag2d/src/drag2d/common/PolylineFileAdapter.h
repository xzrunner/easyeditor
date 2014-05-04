#pragma once

#include <vector>
#include <wx/wx.h>

#include "IFileAdapter.h"

namespace d2d
{
	class ChainShape;

	class PolylineFileAdapter : public IFileAdapter
	{
	public:
		PolylineFileAdapter(std::vector<ChainShape*>& chains);

		void setImgPath(const wxString& path) {
			m_imgPath = path;
		}

	public:
		wxString m_imgPath;
		std::vector<ChainShape*>& m_chains;

	}; // PolylineFileAdapter
}

