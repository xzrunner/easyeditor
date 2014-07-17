#include "PixelDiffCMPT.h"
#include "PixelDiffOP.h"
#include "StagePanel.h"

namespace eimage
{

PixelDiffCMPT::PixelDiffCMPT(wxWindow* parent, const wxString& name, 
							 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new PixelDiffOP(stage);	
}

wxSizer* PixelDiffCMPT::initLayout()
{
	return new wxBoxSizer(wxVERTICAL);
}

}