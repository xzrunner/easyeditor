#include "DrawSpritesVisitor.h"
#include "typedef.h"

#include "frame/SettingCfg.h"

namespace lr
{

DrawSpritesVisitor::DrawSpritesVisitor(const d2d::Rect& screen_region, float cam_scale)
	: d2d::DrawSpritesVisitor(screen_region, cam_scale)
{
}

void DrawSpritesVisitor::DrawSprite(d2d::SpriteRenderer* rd, d2d::ISprite* spr) const
{
	if (!SettingCfg::Instance()->m_special_layer_flag) {
		rd->Draw(spr);
		return;
	}

	const std::string& tag = spr->tag;
	if (tag.find(COVER_LAYER_TAG) != std::string::npos) {
		rd->Draw(spr, d2d::Matrix(), d2d::Colorf(189 / 255.0f, 134 / 255.0f, 203 / 255.0f));
	} else if (tag.find(TOP_LAYER_TAG) != std::string::npos) {
		rd->Draw(spr, d2d::Matrix(), d2d::Colorf(3 / 255.0f, 149 / 255.0f, 146 / 255.0f));
	} else {
		rd->Draw(spr);
	}
}

}