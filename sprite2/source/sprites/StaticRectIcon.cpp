#include "StaticRectIcon.h"
#include "Texture.h"

namespace s2
{

StaticRectIcon::StaticRectIcon()
	: m_min(0, 0)
	, m_max(1, 1)
{
}

void StaticRectIcon::SetRegion(const sm::rect& r)
{
	sm::vec2 sz = m_tex->GetSize();
	m_min.x = r.xmin / sz.x + 0.5f;
	m_min.y = r.ymin / sz.y + 0.5f;
	m_max.x = r.xmax / sz.x + 0.5f;
	m_max.y = r.ymax / sz.y + 0.5f;

	m_min.x = std::min(1.0f, std::max(0.0f, m_min.x));
	m_min.y = std::min(1.0f, std::max(0.0f, m_min.y));
	m_max.x = std::max(0.0f, std::min(1.0f, m_max.x));
	m_max.y = std::max(0.0f, std::min(1.0f, m_max.y));
}

void StaticRectIcon::GetQuad(float process, sm::vec2 quad[4]) const
{
	quad[0].Set(m_min.x, m_min.y);
	quad[1].Set(m_min.x, m_max.y);
	quad[2].Set(m_max.x, m_max.y);
	quad[3].Set(m_max.x, m_min.y);
}

}