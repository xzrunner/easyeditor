#include "ImageProcessCMPT.h"
#include "ImageProcessOP.h"
#include "StagePanel.h"

namespace eimage
{

ImageProcessCMPT::ImageProcessCMPT(wxWindow* parent, const wxString& name, 
								   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new ImageProcessOP(stage);
}

wxSizer* ImageProcessCMPT::initLayout()
{
	return NULL;
}

}