#include "ChangedSectorIcon.h"

namespace eicon
{

ChangedSectorIcon::ChangedSectorIcon()
	: m_min(d2d::PI * 0.5f)
	, m_max(0)
{
}

void ChangedSectorIcon::Draw(const d2d::Matrix& mt, float process) const
{
	d2d::Rect curr;
// 	curr.xMin = m_begin.xMin + (m_end.xMin - m_begin.xMin) * process;
// 	curr.xMax = m_begin.xMax + (m_end.xMax - m_begin.xMax) * process;
// 	curr.yMin = m_begin.yMin + (m_end.yMin - m_begin.yMin) * process;
// 	curr.yMax = m_begin.yMax + (m_end.yMax - m_begin.yMax) * process;

		

	Icon::Draw(mt, curr);
}

void ChangedSectorIcon::LoadFromFile(const Json::Value& value)
{

}

void ChangedSectorIcon::StoreToFile(Json::Value& value) const
{

}

void ChangedSectorIcon::GetRegion(float process, d2d::Rect& region) const
{

}

void ChangedSectorIcon::SetRegion(float angle_min, float angle_max)
{

}

}