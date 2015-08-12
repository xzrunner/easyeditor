#include "DrawSpritesVisitor.h"
#include "typedef.h"

#include "frame/SettingCfg.h"

namespace lr
{

DrawSpritesVisitor::DrawSpritesVisitor(const d2d::Rect& screen_region, float cam_scale)
	: d2d::DrawSpritesVisitor(screen_region, cam_scale)
{
}

void DrawSpritesVisitor::DrawSorted() const
{
	d2d::SpriteRenderer* rd = d2d::SpriteRenderer::Instance();
	std::sort(m_need_sort.begin(), m_need_sort.end(), d2d::SpriteCmp(d2d::SpriteCmp::e_y_invert));
	for (int i = 0, n = m_need_sort.size(); i < n; ++i) {
		d2d::ISprite* spr = m_need_sort[i];
		if (SettingCfg::Instance()->m_special_layer_flag) {
			rd->Draw(spr, d2d::Matrix(), d2d::Colorf(189 / 255.0f, 134 / 255.0f, 203 / 255.0f));
		} else {
			rd->Draw(spr);
		}
	}
}

void DrawSpritesVisitor::DrawSprite(d2d::SpriteRenderer* rd, d2d::ISprite* spr) const
{
	bool draw_flag = SettingCfg::Instance()->m_special_layer_flag;

	const std::string& tag = spr->tag;
	if (tag.find(COVER_LAYER_TAG) != std::string::npos) {
//		rd->Draw(spr, d2d::Matrix(), d2d::Colorf(189 / 255.0f, 134 / 255.0f, 203 / 255.0f));
		m_need_sort.push_back(spr);
	} else if (draw_flag && tag.find(TOP_LAYER_TAG) != std::string::npos) {
		rd->Draw(spr, d2d::Matrix(), d2d::Colorf(3 / 255.0f, 149 / 255.0f, 146 / 255.0f));
	} else {
		rd->Draw(spr);
	}
}

}