#include "StagePanel3D.h"

namespace eshader
{

StagePanel3D::StagePanel3D(wxWindow* parent, wxTopLevelWindow* frame, 
						   d2d::LibraryPanel* library)
	: libsketch::StagePanel(parent, frame, library)
	, m_shader(NULL)
{
}

}