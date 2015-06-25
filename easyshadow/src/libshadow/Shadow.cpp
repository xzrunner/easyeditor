#include "Shadow.h"
#include "ShadowShader.h"

namespace eshadow
{

int Shadow::m_shader_idx = -1;

Shadow::Shadow(float radius)
	: m_radius(radius)
{
	m_inner_color.set(0, 0, 0, 1);
	m_outer_color.set(0.5f, 0.5f, 0.5f, 0);
}

Shadow::~Shadow()
{
}

void Shadow::StoreToFile(Json::Value& value) const
{
	d2d::JsonIO::Store(m_inner_loop, value["inner loop"]);
	value["radius"] = m_radius;
	d2d::JsonIO::Store(m_inner_color, value["inner color"]);
	d2d::JsonIO::Store(m_outer_color, value["outer color"]);
}

void Shadow::LoadFromFile(const Json::Value& value)
{
	d2d::JsonIO::Load(value["inner color"], m_inner_color);
	d2d::JsonIO::Load(value["outer color"], m_outer_color);
	m_radius = value["radius"].asDouble();

	std::vector<d2d::Vector> loop;
	d2d::JsonIO::Load(value["inner loop"], loop);
	BuildInnerLine(loop);
}

void Shadow::Draw(const d2d::Matrix& mt, float alpha) const
{
	if (m_shader_idx == -1) {
		InitShader();
	}

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	shader_mgr->SetShapeShader(m_shader_idx);

	shader_mgr->shape();

	ShadowShader* shader = static_cast<ShadowShader*>(shader_mgr->GetShapeShader());
	shader->SetAlpha(alpha);

	d2d::PrimitiveDraw::DrawTriangles(mt, m_tris, m_colors);

// 	d2d::PrimitiveDraw::drawPolyline(mt, m_inner_loop, d2d::LIGHT_RED, true);
// 	d2d::PrimitiveDraw::drawPolyline(mt, m_outer_loop, d2d::LIGHT_GREEN, true);

	shader_mgr->SetShapeShader(0);
}

void Shadow::BuildFace()
{
	if (m_inner_loop.empty()) {
		return;
	}

	m_tris.clear();
	m_colors.clear();

	d2d::Triangulation::normal(m_inner_loop, m_tris);
	m_colors.reserve(m_tris.size() + m_inner_loop.size() * 6);
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_colors.push_back(m_inner_color);
	}

	assert(m_inner_loop.size() == m_outer_loop.size());
	for (int i = 0, n = m_inner_loop.size(); i < n; ++i)
	{
		int next = i == n - 1 ? 0 : i + 1;

		m_tris.push_back(m_inner_loop[i]);
		m_colors.push_back(m_inner_color);

		m_tris.push_back(m_inner_loop[next]);
		m_colors.push_back(m_inner_color);

		m_tris.push_back(m_outer_loop[i]);
		m_colors.push_back(m_outer_color);

		m_tris.push_back(m_outer_loop[i]);
		m_colors.push_back(m_outer_color);

		m_tris.push_back(m_outer_loop[next]);
		m_colors.push_back(m_outer_color);

		m_tris.push_back(m_inner_loop[next]);
		m_colors.push_back(m_inner_color);
	}
}

void Shadow::BuildOutterLine()
{
	BuildOuterLoop();
	BuildFace();
}

void Shadow::BuildInnerLine(const std::vector<d2d::Vector>& loop)
{
	if (loop.empty()) {
		return;
	}

	BuildInnerLoop(loop);
	BuildOutterLine();
}

void Shadow::SetInnerColer(const d2d::Colorf& col) 
{ 
	m_inner_color.r = col.r;
	m_inner_color.g = col.g;
	m_inner_color.b = col.b;
}

void Shadow::SetOuterColer(const d2d::Colorf& col) 
{ 
	m_outer_color.r = col.r; 
	m_outer_color.g = col.g; 
	m_outer_color.b = col.b; 
}

void Shadow::InitShader()
{
	ShadowShader* shader = new ShadowShader;
	shader->Load();
	m_shader_idx = d2d::ShaderMgr::Instance()->AddShapeShader(shader);
}

void Shadow::BuildInnerLoop(const std::vector<d2d::Vector>& loop)
{
	m_inner_loop.clear();
	int sz = loop.size();
	m_inner_loop.reserve(sz);

	float xmin = loop[0].x;
	int leftmost = 0;
	for (int i = 1; i < sz; ++i) {
		if (loop[i].x < leftmost) {
			xmin = loop[i].x;
			leftmost = i;
		}
	}

	const d2d::Vector& prev = loop[(leftmost - 1 + sz) % sz];
	const d2d::Vector& next = loop[(leftmost + 1) % sz];
	if (d2d::Math::IsTurnLeft(prev, loop[leftmost], next)) {
		copy(loop.rbegin(), loop.rend(), back_inserter(m_inner_loop));
	} else {
		m_inner_loop = loop;
	}
}

void Shadow::BuildOuterLoop()
{
	m_region.makeInfinite();

	m_outer_loop.clear();
	int sz = m_inner_loop.size();
	m_outer_loop.reserve(sz);

	for (int i = 0; i < sz; ++i)
	{
		const d2d::Vector& curr(m_inner_loop[i]),
			&prev(m_inner_loop[(i - 1 + sz) % sz]),
			&next(m_inner_loop[(i + 1) % sz]);
		
		d2d::Vector cb, ce, nb, ne;
		d2d::Math::SideOffsetSegment(prev, curr, true, m_radius, cb, ce);
		d2d::Math::SideOffsetSegment(curr, next, true, m_radius, nb, ne);

 		d2d::Vector cross;
		d2d::Math::GetTwoLineCross(cb, ce, nb, ne, &cross);
		m_outer_loop.push_back(cross);

		m_region.combine(cross);
	}
}

}