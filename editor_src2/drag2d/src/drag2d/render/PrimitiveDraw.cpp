#include "PrimitiveDraw.h"
#include "GL10.h"

#include "common/Vector.h"
#include "common/Rect.h"
#include "common/Color.h"
#include "common/Math.h"
#include "render/Shader.h"

namespace d2d
{

void PrimitiveDraw::resetColorAndTexture()
{
	Shader::Instance()->shape();
	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
	GL10::Color4f(1, 1, 1, 1);
}

void PrimitiveDraw::rect(const Vector& center, float radius, const ShapeStyle& style)
{
	rect(center, radius, radius, style);
}

void PrimitiveDraw::rect(const Vector& center, float hWidth, float hHeight, const ShapeStyle& style)
{
	rect(center - Vector(hWidth, hHeight), center + Vector(hWidth, hHeight), style);
}

void PrimitiveDraw::rect(const Rect& r, const ShapeStyle& style)
{
	rect(Vector(r.xMin, r.yMin), Vector(r.xMax, r.yMax), style);
}

void PrimitiveDraw::rect(const Vector& p0, const Vector& p1, const ShapeStyle& style)
{
	Shader* shader = Shader::Instance();
	shader->shape();

	int type = style.fill ? GL10::GL_QUADS : GL10::GL_LINE_LOOP;
	if (!style.fill)
		GL10::LineWidth(style.size);

	shader->color(style.color);
	lineStypeBegin(style.lineStyle);
	GL10::Begin(type);
		GL10::Vertex2f(p0.x, p0.y);
		GL10::Vertex2f(p0.x, p1.y);
		GL10::Vertex2f(p1.x, p1.y);
		GL10::Vertex2f(p1.x, p0.y);
	GL10::End();
	lineStypeEnd(style.lineStyle);

	if (!style.fill)
		GL10::LineWidth(1);
}

void PrimitiveDraw::drawCircle(const Vector& center, float radius, bool isFill/* = false*/, 
							   float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	Shader::Instance()->shape();

	if (!isFill)
	{
		GL10::LineWidth(size);

		GL10::Color4f(color.r, color.g, color.b, color.a);
		const float k_increment = 2.0f * PI / kSegments;
		float theta = 0.0f;
		GL10::Begin(GL10::GL_LINE_LOOP);
		for (size_t i = 0; i < kSegments; ++i)
		{
			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			GL10::Vertex2f(v.x, v.y);
			theta += k_increment;
		}
		GL10::End();

		GL10::LineWidth(1.0f);
	}
	else
	{
		GL10::Color4f(color.r, color.g, color.b, color.a);

		const float k_increment = 2.0f * PI / kSegments;
		float theta = 0.0f;

		std::vector<Vector> vertices;
		std::vector<Colorf> colors;
		vertices.reserve(kSegments + 2);
		colors.reserve(kSegments + 2);
		vertices.push_back(center);
		colors.push_back(color);
		for (size_t i = 0; i < kSegments; ++i)
		{
			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			vertices.push_back(v);
			colors.push_back(color);
			theta += k_increment;
		}
		vertices.push_back(vertices[1]);
//		colors.push_back(color);

		GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
//		GL10::EnableClientState(GL10::GL_COLOR_ARRAY);

		GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &vertices[0]);
//		GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

		GL10::DrawArrays(GL10::GL_TRIANGLE_FAN, 0, vertices.size());

//		GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
		GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);
	}
}

void PrimitiveDraw::drawCircles(const std::vector<Vector>& circles, float radius, bool isFill/* = false*/, 
								float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	Shader::Instance()->shape();

	if (!isFill)
	{
		for (size_t i = 0, n = circles.size(); i < n; ++i)
			drawCircle(circles[i], radius, isFill, size, color, kSegments);
	}
	else
	{
		if (circles.empty()) return;

		GL10::Color4f(color.r, color.g, color.b, color.a);

		const float k_increment = 2.0f * PI / kSegments;

		std::vector<Vector> vertices;
//		std::vector<Colorf> colors;
		const int size = kSegments * circles.size() * 3;

		vertices.reserve(size);
//		colors.resize(size, color);
		for (size_t i = 0, n = circles.size(); i < n; ++i)
		{
			float theta = 0.0f;
			Vector lastPos;
			for (size_t j = 0; j < kSegments; ++j)
			{
				if (j == 0)
					vertices.push_back(circles[i] + Vector(cosf(theta), sinf(theta)) * radius);
				else
					vertices.push_back(lastPos);
				lastPos = circles[i] + Vector(cosf(theta + k_increment), sinf(theta + k_increment)) * radius;
				vertices.push_back(lastPos);
				vertices.push_back(circles[i]);

				theta += k_increment;
			}
		}

// 		GL10::Enable(GL10::GL_BLEND);
// 		GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

		GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
//		GL10::EnableClientState(GL10::GL_COLOR_ARRAY);

		GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &vertices[0]);
//		GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

		GL10::DrawArrays(GL10::GL_TRIANGLES, 0, vertices.size());

//		GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
		GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

		//GL10::Disable(GL10::GL_BLEND);
	}
}

void PrimitiveDraw::drawPoints(const std::vector<Vector>& vertices, const Colorf& color, float size/* = 2*/)
{
	if (vertices.empty()) return;

	Shader::Instance()->shape();

	GL10::PointSize(size);

	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
	GL10::EnableClientState(GL10::GL_COLOR_ARRAY);

	std::vector<Colorf> colors(vertices.size(), color);

	GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &vertices[0]);
	GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

	GL10::DrawArrays(GL10::GL_POINTS, 0, vertices.size());

	GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

	GL10::PointSize(1.0f);
}

void PrimitiveDraw::drawLine(const Vector& p0, const Vector& p1, 
							 const Colorf& color, float size/* = 2*/)
{
	Shader::Instance()->shape();

	GL10::LineWidth(size);

	GL10::Color4f(color.r, color.g, color.b, color.a);

	GL10::Begin(GL10::GL_LINES);
		GL10::Vertex2f(p0.x, p0.y); 
		GL10::Vertex2f(p1.x, p1.y);
	GL10::End();

	GL10::LineWidth(1.0f);
}

void PrimitiveDraw::drawDotLine(const Vector& p0, const Vector& p1, 
								const Colorf& color, float size /*= 2*/)
{
	Shader::Instance()->shape();

	d2d::GL10::Enable(d2d::GL10::GL_LINE_STIPPLE);

	d2d::GL10::LineStipple(1, 0x0101);
	d2d::PrimitiveDraw::drawLine(p0, p1, color, size);

	d2d::GL10::Disable(d2d::GL10::GL_LINE_STIPPLE);
}

void PrimitiveDraw::drawDashLine(const Vector& p0, const Vector& p1, 
								 const Colorf& color, float size /*= 2*/)
{
	Shader::Instance()->shape();

	d2d::GL10::Enable(d2d::GL10::GL_LINE_STIPPLE);

	d2d::GL10::LineStipple(1, 0x00FF);
	d2d::PrimitiveDraw::drawLine(p0, p1, color, size);

	d2d::GL10::Disable(d2d::GL10::GL_LINE_STIPPLE);
}

void PrimitiveDraw::drawDotDashLine(const Vector& p0, const Vector& p1, 
									const Colorf& color, float size /*= 2*/)
{
	Shader::Instance()->shape();

	d2d::GL10::Enable(d2d::GL10::GL_LINE_STIPPLE);

	d2d::GL10::LineStipple(1, 0x1c47);
	d2d::PrimitiveDraw::drawLine(p0, p1, color, size);

	d2d::GL10::Disable(d2d::GL10::GL_LINE_STIPPLE);
}

void PrimitiveDraw::drawLines(const std::vector<Vector>& vertices, 
							  const Colorf& color, float size /*= 2*/)
{
	Shader::Instance()->shape();

	GL10::LineWidth(size);

	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
	GL10::EnableClientState(GL10::GL_COLOR_ARRAY);

	std::vector<Colorf> colors(vertices.size(), color);

	GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &vertices[0]);
	GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

	GL10::DrawArrays(GL10::GL_LINES, 0, vertices.size());

	GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

	GL10::LineWidth(1.0f);
}

void PrimitiveDraw::drawPolyline(const std::vector<Vector>& vertices, 
								 const Colorf& color, bool isClose, float size /*= 2*/)
{
	Shader* shader = Shader::Instance();
	shader->shape();
	shader->color(color);

	GL10::LineWidth(size);

 	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
//  	GL10::EnableClientState(GL10::GL_COLOR_ARRAY);
 
// 	std::vector<Colorf> colors(vertices.size(), color);

	GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &vertices[0]);
//	GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

	if (isClose)
		GL10::DrawArrays(GL10::GL_LINE_LOOP, 0, vertices.size());
	else
		GL10::DrawArrays(GL10::GL_LINE_STRIP, 0, vertices.size());

 	GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
 	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

	GL10::LineWidth(1.0f);
}

void PrimitiveDraw::drawPolygon(const std::vector<Vector>& vertices, const Colorf& color)
{
	Shader::Instance()->shape();

// 	GL10::Enable(GL10::GL_BLEND);
// 	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
	GL10::EnableClientState(GL10::GL_COLOR_ARRAY);

	std::vector<Colorf> colors(vertices.size(), color);

	GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &vertices[0]);
	GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

	GL10::DrawArrays(GL10::GL_POLYGON, 0, vertices.size());

	GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

	//GL10::Disable(GL10::GL_BLEND);
}

void PrimitiveDraw::drawTriangles(const std::vector<Vector>& triangles, const Colorf& color)
{
	Shader::Instance()->shape();

	if (triangles.empty()) return;

	GL10::Color4f(color.r, color.g, color.b, color.a);

// 	GL10::Enable(GL10::GL_BLEND);
// 	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

// 	std::vector<Colorf> colors;
// 	colors.resize(triangles.size(), color);

 	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
// 	GL10::EnableClientState(GL10::GL_COLOR_ARRAY);

	GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &triangles[0]);
//	GL10::ColorPointer(4, GL10::GL_FLOAT, 0, &colors[0]);

	GL10::DrawArrays(GL10::GL_TRIANGLES, 0, triangles.size());

// 	GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
 	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

	//GL10::Disable(GL10::GL_BLEND);
}

//void PrimitiveDraw::drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath)
//{
//	if (triangles.empty()) return;
//
//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//
//	glClearStencil(0);
//	glEnable(GL_STENCIL_TEST);
//
//	glClear(GL_STENCIL_BUFFER_BIT); 
//
// 	glStencilFunc(GL_NEVER, 0x0, 0x0);
// 	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
//	{
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(2, GL_FLOAT, 0, &triangles[0]);
//		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)triangles.size());
//		glDisableClientState(GL_VERTEX_ARRAY);
//	}
// 	glStencilFunc(GL_NOTEQUAL, 0x0, 0x1);
// 	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//	{
//		Symbol* symbol = SymbolMgr::Instance()->getItem(filepath);
//
//		unsigned int id;
//		float width, height;
//		symbol->getTextureInfo(id, width, height);
//
//		Vector leftLow(FLOAT_MAX, FLOAT_MAX), rightTop(-FLOAT_MAX, -FLOAT_MAX);
//		for (size_t i = 0, n = triangles.size(); i < n; ++i)
//		{
//			const Vector& pos = triangles[i];
//			if (pos.x < leftLow.x) leftLow.x = pos.x;
//			if (pos.x > rightTop.x) rightTop.x = pos.x;
//			if (pos.y < leftLow.y) leftLow.y = pos.y;
//			if (pos.y > rightTop.y) rightTop.y = pos.y;
//		}
//
//		const size_t count = std::ceil((rightTop.x - leftLow.x) / width)
//			* std::ceil((rightTop.y - leftLow.y) / height);
//
//		std::vector<Vector> vertices;
//		std::vector<GLushort> indices;
//		vertices.reserve(count * 4 * 2);
//		indices.reserve(count * 6);
//		int index = 0;
//		for (float x = leftLow.x; x < rightTop.x; x += width)
//		{
//			for (float y = leftLow.y; y < rightTop.y; y += height)
//			{
//				vertices.push_back(Vector(x, y));
//				vertices.push_back(Vector(0, 0));
//
//				vertices.push_back(Vector(x + width, y));
//				vertices.push_back(Vector(1, 0));
//
//				vertices.push_back(Vector(x + width, y + height));
//				vertices.push_back(Vector(1, 1));
//
//				vertices.push_back(Vector(x, y + height));
//				vertices.push_back(Vector(0, 1));
//
//				indices.push_back(index + 0);
//				indices.push_back(index + 1);
//				indices.push_back(index + 2);
//
//				indices.push_back(index + 0);
//				indices.push_back(index + 2);
//				indices.push_back(index + 3);
//
//				index += 4;
//			}
//		}
//
//		glBindTexture(GL_TEXTURE_2D, id);
//
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(2, GL_FLOAT, sizeof(float) * 4, &vertices[0]);
//
//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//		glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 4, &vertices[1]);
//
//		glDrawElements(GL_TRIANGLES, vertices.size() * 3 / 4, GL_UNSIGNED_SHORT, &indices[0]);
//
//		glDisableClientState(GL_VERTEX_ARRAY);
//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//
//		glBindTexture(GL_TEXTURE_2D, NULL);
//	}
//	glDisable(GL_STENCIL_TEST);
//}
//
//void PrimitiveDraw::drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath, 
//						   const Vector& leftLow, const const std::vector<unsigned int>& mesh)
//{
//	if (triangles.empty()) return;
//
//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//
//	glClearStencil(0);
//	glEnable(GL_STENCIL_TEST);
//
//	glClear(GL_STENCIL_BUFFER_BIT); 
//
// 	glStencilFunc(GL_NEVER, 0x0, 0x0);
// 	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
//	{
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(2, GL_FLOAT, 0, &triangles[0]);
//		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)triangles.size());
//		glDisableClientState(GL_VERTEX_ARRAY);
//	}
// 	glStencilFunc(GL_NOTEQUAL, 0x0, 0x1);
// 	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//	{
//		Symbol* symbol = SymbolMgr::Instance()->getItem(filepath);
//
//		unsigned int id;
//		float width, height;
//		symbol->getTextureInfo(id, width, height);
//
//		std::vector<Vector> vertices;
//		std::vector<GLushort> indices;
//		vertices.reserve(mesh.size() * 4 * 2);
//		indices.reserve(mesh.size() * 6);
//		int index = 0;
//		for (size_t i = 0, n = mesh.size(); i < n; ++i)
//		{
//			int pos = mesh[i];
//			float left = leftLow.x + (pos & 0x0000ffff) * width;
//			float low = leftLow.y + (pos >> 16) * height;
//
//			vertices.push_back(Vector(left, low));
//			vertices.push_back(Vector(0, 0));
//
//			vertices.push_back(Vector(left + width, low));
//			vertices.push_back(Vector(1, 0));
//
//			vertices.push_back(Vector(left + width, low + height));
//			vertices.push_back(Vector(1, 1));
//
//			vertices.push_back(Vector(left, low + height));
//			vertices.push_back(Vector(0, 1));
//
//			indices.push_back(index + 0);
//			indices.push_back(index + 1);
//			indices.push_back(index + 2);
//
//			indices.push_back(index + 0);
//			indices.push_back(index + 2);
//			indices.push_back(index + 3);
//
//			index += 4;
//		}
//
//		glBindTexture(GL_TEXTURE_2D, id);
//
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(2, GL_FLOAT, sizeof(float) * 4, &vertices[0]);
//
//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//		glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 4, &vertices[1]);
//
//		glDrawElements(GL_TRIANGLES, vertices.size() * 3 / 4, GL_UNSIGNED_SHORT, &indices[0]);
//
//		glDisableClientState(GL_VERTEX_ARRAY);
//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//
//		glBindTexture(GL_TEXTURE_2D, NULL);
//	}
//	glDisable(GL_STENCIL_TEST);
//}

void PrimitiveDraw::drawTriangles(unsigned int texID, const std::vector<Vector>& triangles, 
								  const std::vector<Vector>& texCoords)
{
	if (triangles.empty()) return;

	Shader::Instance()->null();

	GL10::BindTexture(GL10::GL_TEXTURE_2D, texID);

	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
	GL10::EnableClientState(GL10::GL_TEXTURE_COORD_ARRAY);

	GL10::VertexPointer(2, GL10::GL_FLOAT, 0, &triangles[0]);
	GL10::TexCoordPointer(2, GL10::GL_FLOAT, 0, &texCoords[0]);

	GL10::DrawArrays(GL10::GL_TRIANGLES, 0, triangles.size());

	GL10::DisableClientState(GL10::GL_TEXTURE_COORD_ARRAY);
	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);

	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
}
 //
 //void PrimitiveDraw::drawMesh(unsigned int texID, const std::vector<Vector>& vertices, 
 //							 const std::vector<Vector>& texCoords)
 //{
 //	GL10::Enable(GL10::GL_BLEND);
 //	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);
 //
 //	GL10::Color4f(1.0f, 1.0f, 1.0f, 1.0f);
 //
 //	GL10::PushMatrix();
 //
 //	//	glActiveTexture(GL_TEXTURE0);
 //	GL10::BindTexture(GL10::GL_TEXTURE_2D, texID);
 //
 //	GL10::EnableClientState(GL10::GL_VERTEX_ARRAY);
 //	GL10::VertexPointer(2, GL10::GL_FLOAT, sizeof(Vector), &vertices[0]);
 //
 //	GL10::EnableClientState(GL10::GL_TEXTURE_COORD_ARRAY);
 //	GL10::TexCoordPointer(2, GL10::GL_FLOAT, sizeof(Vector), &texCoords[0]);
 //
 //	GL10::DrawArrays(GL10::GL_TRIANGLES, 0, vertices.size());
 //
 //	GL10::DisableClientState(GL10::GL_VERTEX_ARRAY);
	//GL10::DisableClientState(GL10::GL_TEXTURE_COORD_ARRAY);
	//GL10::DisableClientState(GL10::GL_COLOR_ARRAY);
 //
 //	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
 //
 //	GL10::PopMatrix();
 //
 //	GL10::Disable(GL10::GL_BLEND);
 //}

void PrimitiveDraw::drawTrianglesSlow(unsigned int texID, const std::vector<Vector>& triangles, 
									  const std::vector<Vector>& texCoords)
{
	if (triangles.empty()) return;

	Shader::Instance()->shape();

// 	GL10::Enable(GL10::GL_BLEND);
// 	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	GL10::Color4f(1.0f, 1.0f, 1.0f, 1.0f);

	GL10::BindTexture(GL10::GL_TEXTURE_2D, texID);
	GL10::Begin(GL10::GL_TRIANGLES);
	for (size_t i = 0, n = triangles.size(); i < n; ++i)
	{
		GL10::TexCoord2f(texCoords[i].x, texCoords[i].y);
		GL10::Vertex3f(triangles[i].x, triangles[i].y, -1.0f);
	}
	GL10::End();
	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);

	//GL10::Disable(GL10::GL_BLEND);
}

void PrimitiveDraw::drawCross(const Vector& center, float edge, const Colorf& color, float size)
{
	Vector s = center, e = center;
	s.x -= edge;
	e.x += edge;
	drawLine(s, e, color, size);

	s  = e = center;
	s.y -= edge;
	e.y += edge;
	drawLine(s, e, color, size);
}

//////////////////////////////////////////////////////////////////////////

void PrimitiveDraw::lineStypeBegin(const LineStype& style)
{
	if (style == LS_DEFAULT)
		return;

	d2d::GL10::Enable(d2d::GL10::GL_LINE_STIPPLE);
	switch (style)
	{
	case LS_DOT:
		d2d::GL10::LineStipple(1, 0x0101);
		break;
	case LS_DASH:
		d2d::GL10::LineStipple(1, 0x00FF);
		break;
	case LS_DOT_DASH:
		d2d::GL10::LineStipple(1, 0x1c47);
		break;
	}
}

void PrimitiveDraw::lineStypeEnd(const LineStype& style)
{
	if (style == LS_DEFAULT)
		return;
	d2d::GL10::Disable(d2d::GL10::GL_LINE_STIPPLE);
}

} // d2d