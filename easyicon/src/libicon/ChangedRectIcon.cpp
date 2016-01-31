#include "ChangedRectIcon.h"

namespace eicon
{

ChangedRectIcon::ChangedRectIcon()
{
	m_begin.xmin = m_end.xmin = 0;
	m_begin.xmax = m_end.xmax = 1;
	m_begin.ymin = m_begin.ymax = 0;
	m_end.ymin = 0;
	m_end.ymax = 1;
}

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	ee::JsonSerializer::Load(value["begin"], m_begin);
	ee::JsonSerializer::Load(value["end"], m_end);
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	ee::JsonSerializer::Store(m_begin, value["begin"]);
	ee::JsonSerializer::Store(m_end, value["end"]);
}

void ChangedRectIcon::GetBound(float process, ee::Vector bound[4]) const
{
	float xmin = m_begin.xmin + (m_end.xmin - m_begin.xmin) * process,
		xmax = m_begin.xmax + (m_end.xmax - m_begin.xmax) * process,
		ymin = m_begin.ymin + (m_end.ymin - m_begin.ymin) * process,
		ymax = m_begin.ymax + (m_end.ymax - m_begin.ymax) * process;
	bound[0].Set(xmin, ymin);
	bound[1].Set(xmin, ymax);
	bound[2].Set(xmax, ymax);
	bound[3].Set(xmax, ymin);
}

}