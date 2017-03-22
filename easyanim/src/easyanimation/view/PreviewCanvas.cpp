#include "PreviewCanvas.h"

#include "dataset/DataMgr.h"
#include "dataset/LayersMgr.h"
#include "message/messages.h"

#include <ee/color_config.h>
#include <ee/PlayControl.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>

#include <easyanim.h>
#include <easyparticle3d.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/Particle3d.h>
#include <sprite2/CameraType.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimCurr& curr)
	: ee::CameraCanvas(stage_wnd, stage, s2::CAM_ORTHO2D, glctx)
	, m_curr(curr)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	s2::RVG::SetColor(ee::LIGHT_GREY);
	s2::RVG::Cross(sm::vec2(0,0), xedge, yedge);

	DrawStageData();
}

void PreviewCanvas::DrawStageData() const
{
	s2::Particle3d::Instance()->BufferClear();
	m_curr.Draw(s2::RenderParams());
	s2::Particle3d::Instance()->Draw();
}

} // eanim