#include "StageCanvas.h"
#include "StagePanel.h"

namespace eanim3d
{

StageCanvas::StageCanvas(d2d::EditPanel* edit_panel)
	: e3d::StageCanvas(edit_panel)
{
}

void StageCanvas::onSize(int w, int h)
{
	e3d::StageCanvas::onSize(w, h);
	m_screen_width = w;
	m_screen_height = h;
}

void StageCanvas::OnDraw()
{
	e3d::ShaderMgr::Instance()->SetModelView(GetCamera3().GetModelViewMat());
	DrawBackground();
	DrawSprites();

	m_stage->drawEditTemp();
}

}