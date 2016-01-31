#include "StagePanel3D.h"
#include "StageCanvas3D.h"

namespace eshader
{

StagePanel3D::StagePanel3D(wxWindow* parent, wxTopLevelWindow* frame, 
						   ee::LibraryPanel* library)
	: libsketch::StagePanel(parent, frame, library)
	, m_shader(NULL)
{
}

}