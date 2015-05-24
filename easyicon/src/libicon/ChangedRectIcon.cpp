#include "ChangedRectIcon.h"

namespace eicon
{

ChangedRectIcon::ChangedRectIcon()
	: m_proc(1)
{
	m_begin.xMin = m_end.xMin = 0;
	m_begin.xMax = m_end.xMax = 1;
	m_begin.yMin = m_begin.yMax = 0;
	m_end.yMin = 0;
	m_end.yMax = 1;

	m_curr = m_begin;
}

void ChangedRectIcon::Draw(const d2d::Matrix& mt) const
{
	Icon::Draw(mt, m_curr);
}

void ChangedRectIcon::SetProcess(float proc)
{
	m_proc = proc;
	m_curr.xMin = m_begin.xMin + (m_end.xMin - m_begin.xMin) * proc;
	m_curr.xMax = m_begin.xMax + (m_end.xMax - m_begin.xMax) * proc;
	m_curr.yMin = m_begin.yMin + (m_end.yMin - m_begin.yMin) * proc;
	m_curr.yMax = m_begin.yMax + (m_end.yMax - m_begin.yMax) * proc;
}

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	d2d::JsonTools::Load(value["begin"], m_begin);
	d2d::JsonTools::Load(value["end"], m_end);
	SetProcess(value["process"].asDouble());
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	d2d::JsonTools::Store(m_begin, value["begin"]);
	d2d::JsonTools::Store(m_end, value["end"]);
	value["process"] = m_proc;
}

void ChangedRectIcon::OnImageChanged()
{
	if (!m_img) {
		return;
	}

	float xlen = m_img->originWidth();
	m_region.xMin = xlen * (m_curr.xMin - 0.5f);
	m_region.xMax = xlen * (m_curr.xMax - 0.5f);

	float ylen = m_img->originHeight();
	m_region.yMin = ylen * (m_curr.yMin - 0.5f);
	m_region.yMax = ylen * (m_curr.yMax - 0.5f);	
}

}