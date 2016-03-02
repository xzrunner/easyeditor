#include "EE_RVG.h"
#include "Vector.h"
#include "Color.h"
#include "trans_color.h"
#include "ShaderMgr.h"

#include <rvg.h>
#include <shaderlab.h>

namespace ee
{

void RVG::Init()
{
	rvg_style_init();
}

void RVG::Color(const Colorf& color)
{
	sl_shape_color(color2int(color, PT_ABGR));
}

void RVG::PointSize(float size)
{
	rvg_point_size(size);
}

void RVG::LineWidth(float width)
{
	rvg_line_width(width);
}

void RVG::Point(const Vector& vertex)
{
	rvg_point(vertex.x, vertex.y);
}

void RVG::Points(const std::vector<Vector>& vertices)
{
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		rvg_point(vertices[i].x, vertices[i].y);
	}
}

void RVG::Line(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DEFAULT);
	rvg_line(p0.x, p0.y, p1.x, p1.y);
}

void RVG::DotLine(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DOT);
	rvg_line(p0.x, p0.y, p1.x, p1.y);
}

void RVG::DashLine(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DASH);
	rvg_line(p0.x, p0.y, p1.x, p1.y);
}

void RVG::DotDashLine(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DOT_DASH);
	rvg_line(p0.x, p0.y, p1.x, p1.y);
}

void RVG::Lines(const std::vector<Vector>& vertices)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	rvg_lines(&vertices[0].x, vertices.size());
}

void RVG::Polyline(const std::vector<Vector>& vertices, bool loop)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	rvg_polyline(&vertices[0].x, vertices.size(), loop);
}

void RVG::Triangles(const std::vector<Vector>& triangles)
{
	if (triangles.size() >= 3) {
		rvg_triangles(&triangles[0].x, triangles.size());
	}
}

void RVG::Triangles(const std::vector<Vector>& triangles, const std::vector<Colorf>& colors)
{
	// todo
}

void RVG::Rect(const Vector& center, float hw, float hh, bool filling)
{
	Rect(center - Vector(hw, hh), center + Vector(hw, hh), filling);
}

void RVG::Rect(const Vector& p0, const Vector& p1, bool filling)
{
	ShaderMgr::Instance()->shape();

	rvg_rect(p0.x, p0.y, p1.x, p1.y, filling);

	sl_shader_flush();
}

void RVG::Circle(const Vector& center, float radius, bool filling, int segments)
{
	rvg_circle(center.x, center.y, radius, filling, segments);
}

void RVG::Circles(const std::vector<Vector>& circles, float radius, bool filling, int segments)
{
	for (int i = 0, n = circles.size(); i < n; ++i) {
		Circle(circles[i], radius, filling, segments);
	}
}

void RVG::Cross(const Vector& center, float edge)
{
	Cross(center, edge, edge);
}

void RVG::Cross(const Vector& center, float xedge, float yedge)
{
	Vector s = center, e = center;
	s.x -= xedge;
	e.x += xedge;
	Line(s, e);

	s  = e = center;
	s.y -= yedge;
	e.y += yedge;
	Line(s, e);
}

}