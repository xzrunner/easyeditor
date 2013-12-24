#pragma once

#include <wx/wx.h>
#include <vector>

#include "IFileAdapter.h"

#include "common/Vector.h"

namespace d2d
{
	class MeshFileAdapter : public IFileAdapter
	{
	public:
		virtual void load(const char* filename);

	public:
		wxString m_imagePath;

		std::vector<Vector> m_vertices;
		std::vector<Vector> m_texCoords;

	}; // MeshFileAdapter
}

