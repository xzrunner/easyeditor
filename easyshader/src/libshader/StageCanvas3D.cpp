#include "StageCanvas3D.h"

namespace eshader
{

BEGIN_EVENT_TABLE(StageCanvas3D, libsketch::StageCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas3D::OnTimer)
END_EVENT_TABLE()

StageCanvas3D::StageCanvas3D(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
							 d2d::MultiSpritesImpl* stage, d2d::LibraryPanel* library)
	: libsketch::StageCanvas(edit_panel, stage, library)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas3D::OnTimer(wxTimerEvent& event)
{
	Refresh();
}

}