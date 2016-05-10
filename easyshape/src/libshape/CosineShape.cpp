#include "CosineShape.h"

#include <ee/EE_RVG.h>
#include <ee/DouglasPeucker.h>
#include <ee/SettingData.h>
#include <ee/CosineSmooth.h>

namespace eshape
{

#define SIMPLIFY

const float CosineShape::SAMPLING_WIDTH = 10.0f;

CosineShape::CosineShape()
{
}

CosineShape::CosineShape(const CosineShape& curve)
	: ChainShape(curve)
{
	m_draw_dir = false;

	m_midPoints = curve.m_midPoints;
}

CosineShape::CosineShape(const std::vector<sm::vec2>& vertices)
	: ChainShape(vertices, false)
{
	m_draw_dir = false;

	setMidPoints();
}

CosineShape::~CosineShape()
{
	
}

CosineShape* CosineShape::Clone() const
{
	return new CosineShape(*this);	
}

void CosineShape::Translate(const sm::vec2& offset)
{
	ChainShape::Translate(offset);
	for (int i = 0, n = m_midPoints.size(); i < n; ++i) {
		m_midPoints[i] += offset;
	}
}

void CosineShape::Draw(const sm::mat4& mt, const ee::RenderColor& color) const
{
	if (m_vertices.empty()) return;

	ee::RVG::Color(color.mul);
	ee::RVG::Polyline(m_midPoints, m_loop);
	if (ee::SettingData::ctl_pos_sz != 0) {
		ee::RVG::Color(s2::Color(102, 204, 102));
		ee::RVG::Circles(m_vertices, static_cast<float>(ee::SettingData::ctl_pos_sz), true);
	}
	if (ee::SettingData::ctl_pos_sz != 0) {
		ee::RVG::Color(s2::Color(204, 204, 102));
		ee::RVG::Circles(m_midPoints, ee::SettingData::ctl_pos_sz * 0.5f, true);
	}
}

ee::PropertySetting* CosineShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return NULL;
}

void CosineShape::insertVertices(size_t index, const sm::vec2& pos)
{
	ChainShape::Add(index, pos);
	setMidPoints();
}

void CosineShape::removeVertices(const sm::vec2& pos)
{
	ChainShape::Remove(pos);
	setMidPoints();
}

void CosineShape::changeVertices(const sm::vec2& from, const sm::vec2& to)
{
	ChainShape::Change(from, to);
	setMidPoints();
}

void CosineShape::refresh()
{
//	createBody();
}

void CosineShape::setMidPoints()
{
	m_midPoints.clear();

	if (m_vertices.size() <= 1) return;

	std::vector<sm::vec2> smooth;
	ee::CosineSmooth::Do(m_vertices, SAMPLING_WIDTH, smooth);

#ifdef SIMPLIFY
	ee::DouglasPeucker::Do(smooth, 0.75f, m_midPoints);
#endif
}

}