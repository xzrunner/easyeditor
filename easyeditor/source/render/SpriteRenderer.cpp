#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Config.h"
#include "SettingData.h"
#include "color_config.h"

#include <sprite2/S2_RVG.h>
#include <sprite2/BoundingBox.h>
#include <sprite2/DrawNode.h>

namespace ee
{

SINGLETON_DEFINITION(SpriteRenderer)

static 
void draw_anchor(const s2::Sprite* spr, const s2::RenderParams& params)
{
	const Sprite* ee_spr = dynamic_cast<const Sprite*>(spr);
	if (!ee_spr || !ee_spr->IsAnchor() || !Config::Instance()->GetSettings().draw_anchor) {
		return;
	}

	std::vector<sm::vec2> bound;
	sm::rect rect = spr->GetBounding()->GetSize();
	bound.push_back(sm::vec2(rect.xmin, rect.ymin));
	bound.push_back(sm::vec2(rect.xmax, rect.ymin));
	bound.push_back(sm::vec2(rect.xmax, rect.ymax));
	bound.push_back(sm::vec2(rect.xmin, rect.ymax));		
	for (int i = 0, n = bound.size(); i < n; ++i) {
		bound[i] = params.mt * bound[i];
	}
	s2::RVG::SetColor(BLACK);
	s2::RVG::LineWidth(4);
	s2::RVG::Polyline(bound, true);
	s2::RVG::Line(bound[0], bound[2]);
	s2::RVG::Line(bound[1], bound[3]);
}

SpriteRenderer::SpriteRenderer() 
{
	s2::DrawNode::InitCB(draw_anchor);
}

SpriteRenderer::~SpriteRenderer() 
{
}

void SpriteRenderer::Draw(const s2::Sprite* spr, const s2::RenderParams& params, bool scissor)
{
	s2::DrawNode::Draw(spr, params, scissor);
}

void SpriteRenderer::Draw(const s2::Symbol* sym, const s2::RenderParams& params, 
						  const sm::vec2& pos, float angle, 
						  const sm::vec2& scale, const sm::vec2& shear)
{
	s2::DrawNode::Draw(sym, params, pos, angle, scale, shear);
}

//void SpriteRenderer::InvalidRect(const s2::Sprite* spr, const sm::mat4& mt)
//{
//	if (!spr->IsVisible()) {
//		return;
//	}
//
////	spr->GetSymbol()->InvalidRect(spr->GetTransMatrix() * mt);
//}

}