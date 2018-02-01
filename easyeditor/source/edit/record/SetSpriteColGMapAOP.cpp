#include "SetSpriteColGMapAOP.h"

namespace ee
{

SetSpriteColGMapAOP::SetSpriteColGMapAOP(const SprPtr& spr, const pt2::Color& color)
	: m_new_color(color)
{
	m_sprs.push_back(spr);
	m_old_color.push_back(spr->GetColorMap().gmap);
}

SetSpriteColGMapAOP::SetSpriteColGMapAOP(const std::vector<SprPtr>& sprs, const pt2::Color& color)
	: m_new_color(color)
	, m_sprs(sprs)
{
	for (auto& spr : m_sprs) {
		m_old_color.push_back(spr->GetColorMap().gmap);
	}
}

void SetSpriteColGMapAOP::Undo()
{
	assert(m_sprs.size() == m_old_color.size());
	for (auto& spr : m_sprs) {

	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetColorCommon(pt2::RenderColorCommon(
			m_old_color[i], m_sprs[i]->GetColorMap().gmap
		));
	}
}

void SetSpriteColGMapAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetColorCommon(pt2::RenderColorCommon(
			m_new_color, spr->GetColorMap().gmap
		));
	}
}

Json::Value SetSpriteColGMapAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value ret;
	return ret;
}

}