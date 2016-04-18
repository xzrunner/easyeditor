#include "EE_RVG.h"
#include "Vector.h"
#include "Color.h"
#include "trans_color.h"
#include "CameraMgr.h"

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
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl_shape_color(color2int(color, PT_ABGR));
	} else {
		sl_shape3_color(color2int(color, PT_ABGR));
	}
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
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_point(vertex.x, vertex.y);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_point3(vertex.x, vertex.y, 0);
	}
}

void RVG::Points(const std::vector<Vector>& vertices)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			rvg_point(vertices[i].x, vertices[i].y);
		}
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			rvg_point3(vertices[i].x, vertices[i].y, 0);
		}
	}
}

void RVG::Line(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DEFAULT);

	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::DotLine(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DOT);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::DashLine(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DASH);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::DotDashLine(const Vector& p0, const Vector& p1)
{
	rvg_line_style(LS_DOT_DASH);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::Lines(const std::vector<Vector>& vertices)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_lines(&vertices[0].x, vertices.size());
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		std::vector<vec3> vertices3;
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			vertices3.push_back(vec3(vertices[i].x, vertices[i].y, 0));
		}
		rvg_lines(&vertices3[0].x, vertices3.size());
	}
}

void RVG::Polyline(const std::vector<Vector>& vertices, bool loop)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_polyline(&vertices[0].x, vertices.size(), loop);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		std::vector<vec3> vertices3;
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			vertices3.push_back(vec3(vertices[i].x, vertices[i].y, 0));
		}
		rvg_polyline3(&vertices3[0].x, vertices3.size(), loop);
	}
}

void RVG::Triangles(const std::vector<Vector>& triangles)
{
	if (triangles.size() < 3) return;

	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_triangles(&triangles[0].x, triangles.size());
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		std::vector<vec3> triangles3;
		for (int i = 0, n = triangles.size(); i < n; ++i) {
			triangles3.push_back(vec3(triangles[i].x, triangles[i].y, 0));
		}
		rvg_triangles3(&triangles3[0].x, triangles3.size());
	}
}

void RVG::Triangles(const std::vector<Vector>& triangles, const std::vector<Colorf>& colors)
{
	// todo
}

void RVG::TriangleStrip(const std::vector<Vector>& triangles)
{
	if (triangles.size() < 3) return;

	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_triangle_strip(&triangles[0].x, triangles.size());
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		std::vector<vec3> triangles3;
		for (int i = 0, n = triangles.size(); i < n; ++i) {
			triangles3.push_back(vec3(triangles[i].x, triangles[i].y, 0));
		}
		rvg_triangle_strip3(&triangles3[0].x, triangles3.size());
	}
}

void RVG::Rect(const Vector& center, float hw, float hh, bool filling)
{
	Rect(center - Vector(hw, hh), center + Vector(hw, hh), filling);
}

void RVG::Rect(const Vector& p0, const Vector& p1, bool filling)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_rect(p0.x, p0.y, p1.x, p1.y, filling);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_rect3(p0.x, p0.y, p1.x, p1.y, 0, filling);
	}
}

void RVG::Circle(const Vector& center, float radius, bool filling, int segments)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE);
		rvg_circle(center.x, center.y, radius, filling, segments);
	} else {
		ShaderMgr::Instance()->SetShader(ShaderMgr::SHAPE3);
		rvg_circle3(center.x, center.y, 0, radius, filling, segments);
	}
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