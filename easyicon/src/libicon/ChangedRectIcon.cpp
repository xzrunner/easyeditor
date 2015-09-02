#include "ChangedRectIcon.h"

namespace eicon
{

ChangedRectIcon::ChangedRectIcon()
{
	m_begin.xMin = m_end.xMin = 0;
	m_begin.xMax = m_end.xMax = 1;
	m_begin.yMin = m_begin.yMax = 0;
	m_end.yMin = 0;
	m_end.yMax = 1;
}

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	d2d::JsonIO::Load(value["begin"], m_begin);
	d2d::JsonIO::Load(value["end"], m_end);
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	d2d::JsonIO::Store(m_begin, value["begin"]);
	d2d::JsonIO::Store(m_end, value["end"]);
}

void ChangedRectIcon::GetBound(float process, d2d::Vector bound[4]) const
{
	float xmin = m_begin.xMin + (m_end.xMin - m_begin.xMin) * process,
		xmax = m_begin.xMax + (m_end.xMax - m_begin.xMax) * process,
		ymin = m_begin.yMin + (m_end.yMin - m_begin.yMin) * process,
		ymax = m_begin.yMax + (m_end.yMax - m_begin.yMax) * process;
	bound[0].set(xmin, ymin);
	bound[1].set(xmin, ymax);
	bound[2].set(xmax, ymax);
	bound[3].set(xmax, ymin);
}

}