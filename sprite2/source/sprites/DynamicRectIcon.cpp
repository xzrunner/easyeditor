#include "DynamicRectIcon.h"

namespace s2
{

DynamicRectIcon::DynamicRectIcon()
{
	m_begin.xmin = m_end.xmin = 0;
	m_begin.xmax = m_end.xmax = 1;
	m_begin.ymin = m_begin.ymax = 0;
	m_end.ymin = 0;
	m_end.ymax = 1;
}

void DynamicRectIcon::GetQuad(float process, sm::vec2 quad[4]) const
{
	float xmin = m_begin.xmin + (m_end.xmin - m_begin.xmin) * process,
		  xmax = m_begin.xmax + (m_end.xmax - m_begin.xmax) * process,
		  ymin = m_begin.ymin + (m_end.ymin - m_begin.ymin) * process,
		  ymax = m_begin.ymax + (m_end.ymax - m_begin.ymax) * process;
	quad[0].Set(xmin, ymin);
	quad[1].Set(xmin, ymax);
	quad[2].Set(xmax, ymax);
	quad[3].Set(xmax, ymin);
}

}