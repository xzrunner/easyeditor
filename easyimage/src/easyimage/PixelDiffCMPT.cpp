#include "PixelDiffCMPT.h"
#include "PixelDiffOP.h"
#include "StagePanel.h"

namespace eimage
{

PixelDiffCMPT::PixelDiffCMPT(wxWindow* parent, const wxString& name, 
							 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
{
	m_editOP = new PixelDiffOP(stage);	
}

wxSizer* PixelDiffCMPT::InitLayout()
{
	return new wxBoxSizer(wxVERTICAL);
}

}