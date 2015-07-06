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

void ChangedRectIcon::Draw(const d2d::Matrix& mt, float process) const
{
	d2d::Rect curr;
	curr.xMin = m_begin.xMin + (m_end.xMin - m_begin.xMin) * process;
	curr.xMax = m_begin.xMax + (m_end.xMax - m_begin.xMax) * process;
	curr.yMin = m_begin.yMin + (m_end.yMin - m_begin.yMin) * process;
	curr.yMax = m_begin.yMax + (m_end.yMax - m_begin.yMax) * process;

	d2d::Vector region[4];
	region[0].set(curr.xMin, curr.yMin);
	region[1].set(curr.xMin, curr.yMax);
	region[2].set(curr.xMax, curr.yMax);
	region[3].set(curr.xMax, curr.yMin);

	Icon::Draw(mt, region);
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

void ChangedRectIcon::GetRegion(float process, d2d::Rect& region) const
{
	if (!m_img) {
		return;
	}

 	d2d::Rect curr = GetCurrRect(process);
	d2d::Rect clip_r = m_img->GetClippedRegion();
	region.xMin = clip_r.xMin + curr.xMin * clip_r.xLength();
	region.xMax = clip_r.xMin + curr.xMax * clip_r.xLength();
	region.yMin = clip_r.yMin + curr.yMin * clip_r.yLength();
	region.yMax = clip_r.yMin + curr.yMax * clip_r.yLength();
}

d2d::Rect ChangedRectIcon::GetCurrRect(float process) const
{
	d2d::Rect curr;
	curr.xMin = m_begin.xMin + (m_end.xMin - m_begin.xMin) * process;
	curr.xMax = m_begin.xMax + (m_end.xMax - m_begin.xMax) * process;
	curr.yMin = m_begin.yMin + (m_end.yMin - m_begin.yMin) * process;
	curr.yMax = m_begin.yMax + (m_end.yMax - m_begin.yMax) * process;
	return curr;	
}

void ChangedRectIcon::GetTexcoords4(d2d::Vector tex4[4], float process) const
{
	d2d::Rect curr = GetCurrRect(process);
	tex4[0].set(curr.xMin, curr.yMin);
	tex4[1].set(curr.xMin, curr.yMax);
	tex4[2].set(curr.xMax, curr.yMax);
	tex4[3].set(curr.xMax, curr.yMin);
}

}