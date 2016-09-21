#include "DynamicSectorIcon.h"
#include "ImageSymbol.h"

#include <sm_const.h>

#include <assert.h>

namespace s2
{

DynamicSectorIcon::DynamicSectorIcon()
	: m_min(SM_PI * 0.5f)
	, m_max(0)
{
}

void DynamicSectorIcon::GetQuad(float process, sm::vec2 quad[4]) const
{
	quad[0].Set(0, 0);
	quad[1].Set(0, 1);

	float angle = m_min + (m_max - m_min) * process;
	float tan_val = tan(angle);

	sm::vec2 sz = m_img->GetNoTrimedSize();
	float x = sz.y / tan_val;
	if (x < sz.x) {
		quad[2].Set(x / sz.x, 1.0f);
		quad[3] = quad[2];
	} else {
		float y = sz.x * tan_val;
		assert(y <= sz.y);
		quad[2].Set(1, 1);
		quad[3].Set(1, y / sz.y);
	}
}

void DynamicSectorIcon::SetRegion(float min, float max)
{
	m_min = min;
	m_max = max;
}

void DynamicSectorIcon::GetRegion(float& min, float& max) const
{
	min = m_min;
	max = m_max;
}

}