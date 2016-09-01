#include "PreviewStage.h"

namespace ecomplex
{

PreviewStage::PreviewStage(wxWindow* parent, wxTopLevelWindow* frame, 
						   ee::PlayControl& control)
	: ee::EditPanel(parent, frame)
	, m_control(control)
{
}

bool PreviewStage::UpdateStage()
{
	m_control.Update();
	return true;
}

}