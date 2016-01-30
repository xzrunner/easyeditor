#include "PreviewStage.h"

namespace ecomplex
{

PreviewStage::PreviewStage(wxWindow* parent, wxTopLevelWindow* frame, 
						   d2d::PlayControl& control)
	: d2d::EditPanel(parent, frame)
	, m_control(control)
{
}

bool PreviewStage::Update(int version)
{
	m_control.Update();
	return true;
}

}