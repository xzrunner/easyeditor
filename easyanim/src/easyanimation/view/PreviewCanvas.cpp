#include "PreviewCanvas.h"
#include "PreviewSettings.h"
#include "PreviewUtility.h"

#include "dataset/DataMgr.h"
#include "dataset/LayersMgr.h"
#include "message/messages.h"

#include <ee/EE_RVG.h>
#include <ee/color_config.h>
#include <ee/PlayControl.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>

#include <easyanim.h>
#include <easyparticle3d.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
							 const PlaySettings& settings, ee::PlayControl& control,
							 wxGLContext* glctx)
	: ee::OrthoCanvas(stage_wnd, stage, glctx)
	, m_control(control)
	, m_settings(settings)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	ee::RVG::Color(ee::LIGHT_GREY);
	ee::RVG::Cross(ee::Vector(0,0), xedge, yedge);

	DrawStageData();
}

void PreviewCanvas::OnTimer()
{
	m_control.Update();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	int frame_idx = m_control.Frame();
	if (frame_idx <= DataMgr::Instance()->GetLayers().GetFrameCount()) {
		return;
	}

	if (m_settings.isCirculate) {
		m_control.Reset();
	} else {
		m_control.Decrease();
	}
}

// todo: waste time!
void PreviewCanvas::DrawStageData() const
{
	eparticle3d::PS::Instance()->BufferClear();

	std::vector<ee::Sprite*> sprites;
	PreviewUtility::GetCurrSprites(m_control, sprites);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		if (eparticle3d::Sprite* p3d = dynamic_cast<eparticle3d::Sprite*>(sprites[i])) {
			p3d->OnActive();
		}
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		spr->Update(0);
		ee::SpriteRenderer::Instance()->Draw(spr);
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Release();
	}

	ee::SceneNodeMgr::Instance()->Draw();
}

} // eanim