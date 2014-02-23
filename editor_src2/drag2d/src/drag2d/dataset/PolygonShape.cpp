#include "PolygonShape.h"
#include "ImageSymbol.h"

#include "render/PrimitiveDraw.h"
#include "common/Settings.h"
#include "common/Math.h"

namespace d2d
{

PolygonShape::PolygonShape(const PolygonShape& polygon)
	: ChainShape(polygon)
{
	m_fillingType = polygon.m_fillingType;

	m_fillingColor = polygon.m_fillingColor;
	m_fillingTexture = polygon.m_fillingTexture;

	m_fillingVertices = polygon.m_fillingVertices;
	m_fillingTexCoords = polygon.m_fillingTexCoords;
}

PolygonShape::PolygonShape(const std::vector<Vector>& vertices)
	: ChainShape(vertices, true)
	, m_fillingType(e_Color)
	, m_fillingColor(0.8f, 0.8f, 0.8f, 0.8f)
	, m_fillingTexture(NULL)
{
	buildFillingTris();
}

PolygonShape::~PolygonShape()
{
	clearUserData(true);
}

PolygonShape* PolygonShape::clone() const
{
	return new PolygonShape(*this);
}

void PolygonShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	PrimitiveDraw::resetColorAndTexture();

	if (m_fillingType == e_Color)
		PrimitiveDraw::drawTriangles(m_fillingVertices, m_fillingColor);
	else if (m_fillingType == e_Texture)
		PrimitiveDraw::drawTriangles(m_fillingTexture->getTextureID(), 
		m_fillingVertices, m_fillingTexCoords);
	if (Settings::bDisplayTrisEdge)
	{
		std::vector<Vector> buf;
		for (size_t i = 0, n = m_fillingVertices.size(); i < n; ++i)
		{
			buf.push_back(m_fillingVertices[i]);
			if (buf.size() == 3)
			{
				PrimitiveDraw::drawPolyline(buf, Colorf(0, 1, 0), true);
				buf.clear();
			}
		}
	}

	if (Settings::bDisplayPolyBound)
		ChainShape::draw(color);
}

void PolygonShape::refresh()
{
	buildFillingTris();
}

void PolygonShape::buildFillingTris()
{
	m_fillingVertices.clear();
	m_fillingTexCoords.clear();

	std::vector<Vector> boundingFixed;
	Math::removeDuplicatePoints(m_vertices, boundingFixed);

	if (m_fillingType == e_Color)
	{
		// todo
//		Math::triangulatePolygon(boundingFixed, m_fillingVertices, Math::e_Constrained);
	}
	else
	{
		float left, right, low, up;
		getBoundingRegion(boundingFixed, left, right, low, up);

		std::vector<Vector> segments;
		getTextureBoundarySegments(left, right, low, up, segments);

		// todo
//		Math::triangulatePolygon(boundingFixed, segments, m_fillingVertices, Math::e_Constrained);

		computeTextureCoords(left, low);
	}
}

void PolygonShape::buildFillingTris(const std::vector<Vector>& segments)
{
	m_fillingVertices.clear();
	m_fillingTexCoords.clear();

	std::vector<Vector> boundingFixed;
	Math::removeDuplicatePoints(m_vertices, boundingFixed);

	if (m_fillingType == e_Color)
	{
		// todo
//		Math::triangulatePolygon(boundingFixed, segments, m_fillingVertices, Math::e_Constrained);
	}
	else
	{
		float left, right, low, up;
		getBoundingRegion(boundingFixed, left, right, low, up);

		std::vector<Vector> texSegments;
		getTextureBoundarySegments(left, right, low, up, texSegments);

		copy(segments.begin(), segments.end(), back_inserter(texSegments));

		// todo
//		Math::triangulatePolygon(boundingFixed, texSegments, m_fillingVertices, Math::e_Constrained);

		computeTextureCoords(left, low);
	}
}

void PolygonShape::getBoundingRegion(const std::vector<Vector>& bounding, float& left, float& right, float& low, float& up)
{
	left = low = FLT_MAX;
	right = up = -FLT_MAX;
	for (size_t i = 0, n = bounding.size(); i < n; ++i)
	{
		const Vector& pos = bounding[i];
		if (pos.x < left) left = pos.x;
		if (pos.x > right) right = pos.x;
		if (pos.y < low) low = pos.y;
		if (pos.y > up) up = pos.y;
	}
}

void PolygonShape::getTextureBoundarySegments(float left, float right, float low, float up, std::vector<Vector>& segments)
{
	if (!m_fillingTexture) return;

	const int width = m_fillingTexture->getSize().xLength(),
		height = m_fillingTexture->getSize().yLength();

	for (float x = left; x < right; x += width)
	{
		segments.push_back(Vector(x, low - 1));
		segments.push_back(Vector(x, up + 1));
	}
	for (float y = low; y < up; y += height)
	{
		segments.push_back(Vector(left - 1, y));
		segments.push_back(Vector(right + 1, y));
	}
}

void PolygonShape::computeTextureCoords(float left, float low)
{
	if (!m_fillingTexture) return;

	const int width = m_fillingTexture->getSize().xLength(),
		height = m_fillingTexture->getSize().yLength();

	int index = 0;
	for (size_t i = 0, n = m_fillingVertices.size() / 3; i < n; ++i)
	{
		float cx = 0, cy = 0;
		for (size_t j = 0; j < 3; ++j)
		{
			cx += m_fillingVertices[index + j].x;
			cy += m_fillingVertices[index + j].y;
		}
		cx /= 3;
		cy /= 3;

		const int ix = (cx - left) / width,
			iy = (cy - low) / height;
		Vector base;
		base.x = left + width * ix;
		base.y = low + height * iy;

		for (size_t j = 0; j < 3; ++j)
		{
			const float tx = (m_fillingVertices[index + j].x - base.x) / width,
				ty = (m_fillingVertices[index + j].y - base.y) / height;
			m_fillingTexCoords.push_back(Vector(tx, ty));
		}

		index +=  3;
	}
}

} // d2d