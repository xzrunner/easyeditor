#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/EE_RVG.h>
#include <ee/cfg_const.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/Symbol.h>

namespace eshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 wxGLContext* glctx,
						 ee::Sprite* edited,
						 const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		std::vector<ee::Sprite*> sprites;
		m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
		m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
	}
}

void StageCanvas::DrawGuideLines() const
{
	ee::RVG::Color(ee::LIGHT_GREY);
	ee::RVG::Rect(ee::Vector(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		sm::mat4 mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Draw(m_bg, NULL, mat);
	}

	m_shape_impl->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);

	eshape::StageCanvas::DrawGuideLines();

	if (!m_edited) {
		const ee::Symbol& symbol = m_stage->GetSymbol();
		symbol.Draw(sm::mat4());
	}

	m_stage->DrawEditOP();
}

}