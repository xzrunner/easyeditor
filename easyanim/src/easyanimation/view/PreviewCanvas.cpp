#include "PreviewCanvas.h"
#include "PreviewSettings.h"
#include "PreviewUtility.h"

#include "dataset/DataMgr.h"
#include "dataset/LayersMgr.h"
#include "message/messages.h"

#include <easyanim.h>
#include <easyparticle3d.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
							 const PlaySettings& settings, d2d::PlayControl& control,
							 wxGLContext* glctx)
	: d2d::OrthoCanvas(stage_wnd, stage, glctx)
	, m_control(control)
	, m_settings(settings)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::Cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);

	DrawStageData();
}

void PreviewCanvas::OnTimer()
{
	m_control.Update();
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

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

	std::vector<d2d::Sprite*> sprites;
	PreviewUtility::GetCurrSprites(m_control, sprites);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		if (eparticle3d::Sprite* p3d = dynamic_cast<eparticle3d::Sprite*>(sprites[i])) {
			p3d->OnActive();
		}
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::Sprite* spr = sprites[i];
		spr->Update(0);
		d2d::SpriteRenderer::Instance()->Draw(spr);
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Release();
	}

	d2d::SceneNodeMgr::Instance()->Draw();
}

} // eanim