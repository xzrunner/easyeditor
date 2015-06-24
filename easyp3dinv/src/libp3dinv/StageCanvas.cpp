#include "StageCanvas.h"
#include "StagePanel.h"

namespace ep3dinv
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
{
	m_currFrame = 1;
	m_last = -1;
}

void StageCanvas::OnDraw() const
{
	d2d::Rect sr = m_screen.GetRegion();
	m_stage->traverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);

	m_stage->drawEditTemp();
}

void StageCanvas::OnTimer()
{
// 	if (m_last == -1) {
// 		m_last = clock();
// 	} else {
// 		clock_t curr = clock();
// 		int dt = curr - m_last;
// 		m_last = curr;
// 
// 		if (m_stage->m_ps) {
// 			m_stage->m_ps->update((float)dt / CLOCKS_PER_SEC);
// 		}
// 
// 		
// 
// 		//////////////////////////////////////////////////////////////////////////
// 
// 		// reset 
// 
// 		m_control.update();
// 		m_currFrame = m_control.frame();
// 		if (m_currFrame >= max) {
// 			m_currFrame = 1;
// 			m_control.reset();
// 		}
// 	}
}

}