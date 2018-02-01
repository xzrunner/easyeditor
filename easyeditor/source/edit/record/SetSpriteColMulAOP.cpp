#include "SetSpriteColMulAOP.h"

namespace ee
{

SetSpriteColMulAOP::SetSpriteColMulAOP(const SprPtr& spr, const pt2::Color& color)
	: m_new_color(color)
{
	m_sprs.push_back(spr);
	m_old_color.push_back(spr->GetColorCommon().mul);
}

SetSpriteColMulAOP::SetSpriteColMulAOP(const std::vector<SprPtr>& sprs, const pt2::Color& color)
	: m_new_color(color)
	, m_sprs(sprs)
{
	for (auto& spr : m_sprs) {
		m_old_color.push_back(spr->GetColorCommon().mul);
	}
}

void SetSpriteColMulAOP::Undo()
{
	assert(m_sprs.size() == m_old_color.size());
	for (auto& spr : m_sprs) {

	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetColorCommon(pt2::RenderColorCommon(
			m_old_color[i], m_sprs[i]->GetColorCommon().mul
		));
	}
}

void SetSpriteColMulAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetColorCommon(pt2::RenderColorCommon(
			m_new_color, spr->GetColorCommon().mul
		));
	}
}

Json::Value SetSpriteColMulAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value ret;
	return ret;
}

}