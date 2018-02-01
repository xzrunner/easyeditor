#include "SetSpriteColAddAOP.h"

namespace ee
{

SetSpriteColAddAOP::SetSpriteColAddAOP(const SprPtr& spr, const pt2::Color& color)
	: m_new_color(color)
{
	m_sprs.push_back(spr);
	m_old_color.push_back(spr->GetColorCommon().add);
}

SetSpriteColAddAOP::SetSpriteColAddAOP(const std::vector<SprPtr>& sprs, const pt2::Color& color)
	: m_new_color(color)
	, m_sprs(sprs)
{
	for (auto& spr : m_sprs) {
		m_old_color.push_back(spr->GetColorCommon().add);
	}
}

void SetSpriteColAddAOP::Undo()
{
	assert(m_sprs.size() == m_old_color.size());
	for (auto& spr : m_sprs) {

	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetColorCommon(pt2::RenderColorCommon(
			m_old_color[i], m_sprs[i]->GetColorCommon().add
		));
	}
}

void SetSpriteColAddAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetColorCommon(pt2::RenderColorCommon(
			m_new_color, spr->GetColorCommon().add
		));
	}
}

Json::Value SetSpriteColAddAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value ret;
	return ret;
}

}