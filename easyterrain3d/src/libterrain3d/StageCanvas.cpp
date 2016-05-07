#include "StageCanvas.h"

#include "DemoTriNetwork.h"
#include "DemoBruteForce.h"
#include "DemoBruteForceTex.h"
#include "DemoOcean.h"

#include <ee/CameraCanvas.h>

namespace eterrain3d
{

BEGIN_EVENT_TABLE(StageCanvas, ee::CameraCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::OnTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);

	SetBgColor(ee::BLACK);

//	m_demo = new DemoTriNetwork(GetCamera3());
//	m_demo = new DemoBruteForce(GetCamera3());
	m_demo = new DemoBruteForceTex(GetCamera3());
//	m_demo = new DemoOcean(GetCamera3());

	// fixme
	m_demo->Load();
}

void StageCanvas::OnDrawSprites() const
{
//	e3d::DrawCross(sm::vec3(0, 0, 0), sm::vec3(100, 100, 100), ee::LIGHT_RED);

	m_demo->Update();
	m_demo->Draw();

	e3d::LogViewer::Instance()->Draw();
}

void StageCanvas::OnTimer(wxTimerEvent& event)
{
	Refresh();
}

}