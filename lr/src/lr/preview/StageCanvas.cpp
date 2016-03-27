#include "StageCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/PlayControl.h>

namespace lr
{
namespace preview
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control,
						 const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, glctx)
	, m_control(control)
	, m_sprites(sprites)
{
}

void StageCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const ee::Sprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;
		ee::SpriteRenderer::Instance()->Draw(sprite, NULL, ee::Matrix(), sprite->color);
	}
}

void StageCanvas::OnTimer()
{
	m_control.Update();
}

}
}