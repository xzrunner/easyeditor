#include "PreviewCanvas.h"
#include "PreviewSettings.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "message/GetCurrFrameSJ.h"

#include <easyanim.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, const PlaySettings& settings,
							 d2d::PlayControl& control, const LayersMgr& layers)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_settings(settings)
	, m_layers(layers)
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

	int layer, frame;
	GetCurrFrameSJ::Instance()->Get(layer, frame);
	if (frame < m_layers.GetFrameCount()) {
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
	int layer, frame;
	GetCurrFrameSJ::Instance()->Get(layer, frame);

	for (size_t i = 0, n = m_layers.Size(); i < n; ++i)
	{
		Layer* layer = m_layers.GetLayer(i);

		KeyFrame *curr_f = layer->GetCurrKeyFrame(frame),
			     *next_f = layer->GetNextKeyFrame(frame);
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
			assert(frame >= curr_f->GetTime() && frame < next_f->GetTime());
			float process = (float) (frame - curr_f->GetTime()) / (next_f->GetTime() - curr_f->GetTime());
//			libanim::Tools::getTweenSprites(currFrame->getAllSprites(), nextFrame->getAllSprites(), sprites, process);

			curr_f->GetTweenSprite(curr_f, next_f, sprites, process);
		}
	}
}

} // eanim