#include "PixelDiffCMPT.h"
#include "PixelDiffOP.h"
#include "StagePanel.h"

namespace eimage
{

PixelDiffCMPT::PixelDiffCMPT(wxWindow* parent, const std::string& name, 
							 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
{
	m_editop = new PixelDiffOP(stage);	
}

wxSizer* PixelDiffCMPT::InitLayout()
{
	return new wxBoxSizer(wxVERTICAL);
}

}