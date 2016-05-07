#include "DrawUtils.h"

#include <ee/Sprite.h>

namespace emodeling
{

void DrawUtils::DrawBody(Body* body, DrawType dType)
{
	ee::Colorf cFace, cEdge;
	GetBodyColor(body->m_type, dType, cFace, cEdge);

	sm::mat4 mt;
	body->m_sprite->GetTransMatrix(mt);
	body->Draw(mt, cFace, cEdge);
}

void DrawUtils::DrawFixture(Fixture* fixture, 
							DrawType dType, bool onlyFixture)
{
	ee::Colorf cFace, cEdge;
	if (onlyFixture)
		GetFixtureColor(dType, cFace, cEdge);
	else
		GetBodyColor(fixture->m_body->m_type, dType, cFace, cEdge);

	sm::mat4 mt;
	fixture->m_body->m_sprite->GetTransMatrix(mt);
	fixture->Draw(mt, cFace, cEdge);
}

void DrawUtils::GetBodyColor(Body::Type type, DrawType dType,
							 ee::Colorf& cFace, ee::Colorf& cEdge)
{
	switch (type)
	{
	case Body::e_static:
		cFace.Set(0.5f, 0.9f, 0.5f, 0.2f);
		break;
	case Body::e_kinematic:
		cFace.Set(0.5f, 0.5f, 0.9f, 0.2f);
		break;
	case Body::e_dynamic:
		cFace.Set(0.5f, 0.5f, 0.3f, 0.2f);
		break;
	}

	const float rScale = 1.1f,
		gScale = 1.1f,
		bScale = 0.5f;
	switch (dType)
	{
	case e_default:
		break;
	case e_mouseOn:
		cFace.r *= rScale;
		cFace.g *= gScale;
		cFace.b *= bScale;
		break;
	case e_selected:
		{
			ee::Colorf c = cFace;
			c.r *= rScale;
			c.g *= gScale;
			c.b *= bScale;
			cFace = ColorEnlarge(c, 1.1f);
		}
		break;
	}

	cEdge = ColorEnlarge(cFace, 1.05f);
}

void DrawUtils::GetFixtureColor(DrawType type, ee::Colorf& cFace, ee::Colorf& cEdge)
{
	cFace.Set(0.6f, 0.35f, 0.6f, 0.5f);
	switch (type)
	{
	case e_mouseOn:
		cFace = ColorEnlarge(cFace, 1.1f);
		break;
	case e_selected:
		cFace = ColorEnlarge(cFace, 1.2f);
		break;
	}

	cEdge = ColorEnlarge(cFace, 1.1f);
}

ee::Colorf DrawUtils::ColorEnlarge(const ee::Colorf& color, float factor)
{
	ee::Colorf ret = color;
	ret.r = std::min(color.r * factor, 1.0f);
	ret.g = std::min(color.g * factor, 1.0f);
	ret.b = std::min(color.b * factor, 1.0f);
	ret.a = color.a;

	return ret;
}

}