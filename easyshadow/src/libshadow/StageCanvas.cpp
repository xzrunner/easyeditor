#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/CameraType.h>

namespace eshadow
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx,
						 ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::SprPtr> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
}

StageCanvas::~StageCanvas()
{
	if (m_bg) {
		m_bg->RemoveReference();
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalMat().Inverted();
		ee::SpriteRenderer::Instance()->Draw(m_bg, params);
	}

	m_stage->GetSymbol()->Draw(s2::RenderParams());
	m_stage->DrawEditOP();
}

}