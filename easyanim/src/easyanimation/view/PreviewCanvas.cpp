#include "PreviewCanvas.h"
#include "PreviewSettings.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "message/messages.h"

#include <easyanim.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
							 const PlaySettings& settings, d2d::PlayControl& control)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_settings(settings)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);

	DrawStageData();
}

void PreviewCanvas::OnTimer()
{
	m_control.update();
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	int frame_idx = m_control.frame();
	if (frame_idx <= DataMgr::Instance()->GetLayers().GetFrameCount()) {
		return;
	}

	if (m_settings.isCirculate) {
		m_control.reset();
	} else {
		m_control.decrease();
	}
}

// todo: waste time!
void PreviewCanvas::DrawStageData() const
{
	std::vector<d2d::ISprite*> sprites;
	GetCurrSprites(sprites);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->Release();
}

void PreviewCanvas::GetCurrSprites(std::vector<d2d::ISprite*>& sprites) const
{
	int frame_idx = m_control.frame();
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(i);

		KeyFrame *curr_f = layer->GetCurrKeyFrame(frame_idx),
			     *next_f = layer->GetNextKeyFrame(frame_idx);
		if (!curr_f)
			continue;

		if (!curr_f) 
		{
			for (int i = 0, n = curr_f->Size(); i < n; ++i) {
				sprites.push_back(curr_f->GetSprite(i)->Clone());
			}
		}
		else if (!curr_f->HasClassicTween() || !next_f)
		{
			for (int i = 0, n = curr_f->Size(); i < n; ++i) {
				sprites.push_back(curr_f->GetSprite(i)->Clone());
			}
		}
		else
		{
			assert(frame_idx >= curr_f->GetTime() && frame_idx < next_f->GetTime());
			float process = (float) (frame_idx - curr_f->GetTime()) / (next_f->GetTime() - curr_f->GetTime());
//			libanim::Tools::getTweenSprites(currFrame->getAllSprites(), nextFrame->getAllSprites(), sprites, process);

			curr_f->GetTweenSprite(curr_f, next_f, sprites, process);
		}
	}
}

} // eanim