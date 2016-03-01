#include "EE_RVG.h"
#include "Vector.h"
#include "Color.h"
#include "trans_color.h"
#include "ShapeStyle.h"

#include <rvg.h>
#include <shaderlab.h>

namespace ee
{

void RVG::Init()
{
	rvg_style_init();
}

void RVG::Point(const Vector& vertex, const Colorf& color, float size)
{
	sl_shape_color(color2int(color, PT_RGBA));
	rvg_point_size(size);
	rvg_point(vertex.x, vertex.y, size);
}

void RVG::Points(const std::vector<Vector>& vertices, const Colorf& color, float size)
{
	sl_shape_color(color2int(color, PT_RGBA));
	rvg_point_size(size);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		rvg_point(vertices[i].x, vertices[i].y, size);
	}
}

static void _line(const Vector& p0, const Vector& p1, const Colorf& color, float width)
{
	sl_shape_color(color2int(color, PT_RGBA));
	rvg_line_width(width);
	rvg_line(p0.x, p0.y, p1.x, p1.y);
}

void RVG::Line(const Vector& p0, const Vector& p1, const Colorf& color, float width)
{
	rvg_line_style(LS_DEFAULT);
	_line(p0, p1, color, width);
}

void RVG::DotLine(const Vector& p0, const Vector& p1, const Colorf& color, float width)
{
	rvg_line_style(LS_DOT);
	_line(p0, p1, color, width);
}

void RVG::DashLine(const Vector& p0, const Vector& p1, const Colorf& color, float width)
{
	rvg_line_style(LS_DASH);
	_line(p0, p1, color, width);	
}

void RVG::DotDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float width)
{
	rvg_line_style(LS_DOT_DASH);
	_line(p0, p1, color, width);
}

void RVG::Lines(const std::vector<Vector>& vertices, const Colorf& color, float width)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	sl_shape_color(color2int(color, PT_RGBA));
	rvg_line_width(width);

	rvg_lines(&vertices[0].x, vertices.size());
}

void RVG::Polyline(const std::vector<Vector>& vertices, const Colorf& color, bool loop, float width)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	sl_shape_color(color2int(color, PT_RGBA));
	rvg_line_width(width);

	rvg_polyline(&vertices[0].x, vertices.size(), loop);
}

void RVG::Triangles(const std::vector<Vector>& triangles, const Colorf& color)
{
	if (triangles.size() < 3) {
		return;
	}

	sl_shape_color(color2int(color, PT_RGBA));
	rvg_triangles(&triangles[0].x, triangles.size());
}

void RVG::Rect(const Vector& center, float radius, const ShapeStyle& style)
{
	Rect(center, radius, radius, style);
}

void RVG::Rect(const Vector& center, float hw, float hh, const ShapeStyle& style)
{
	Rect(center - Vector(hw, hh), center + Vector(hw, hh), style);
}

void RVG::Rect(const Vector& p0, const Vector& p1, const ShapeStyle& style)
{
	rvg_line_style((RVG_LINE_STYLE)(style.line_style));
	sl_shape_color(color2int(style.color, PT_RGBA));
	rvg_line_width(style.size);
	rvg_rect(p0.x, p0.y, p1.x, p1.y, style.filling);
}

void RVG::Circle(const Vector& center, float radius, bool filling, const Colorf& color, int segments)
{
	sl_shape_color(color2int(color, PT_RGBA));
	rvg_circle(center.x, center.y, radius, filling, segments);
}

void RVG::Circles(const std::vector<Vector>& circles, float radius, bool filling, const Colorf& color, int segments)
{
	for (int i = 0, n = circles.size(); i < n; ++i) {
		Circle(circles[i], radius, filling, color, segments);
	}
}

void RVG::Cross(const Vector& center, float edge, const Colorf& color, float width)
{
	Cross(center, edge, edge, color, width);
}

void RVG::Cross(const Vector& center, float xedge, float yedge, const Colorf& color, float width)
{
	Vector s = center, e = center;
	s.x -= xedge;
	e.x += xedge;
	Line(s, e, color, width);

	s  = e = center;
	s.y -= yedge;
	e.y += yedge;
	Line(s, e, color, width);
}

}