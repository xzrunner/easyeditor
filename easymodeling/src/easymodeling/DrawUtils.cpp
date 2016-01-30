
#include "DrawUtils.h"

using namespace emodeling;

void DrawUtils::drawBody(libmodeling::Body* body, DrawType dType)
{
	d2d::Colorf cFace, cEdge;
	getBodyColor(body->type, dType, cFace, cEdge);

	d2d::Matrix mt;
	body->sprite->GetTransMatrix(mt);
	body->draw(mt, cFace, cEdge);
}

void DrawUtils::drawFixture(libmodeling::Fixture* fixture, 
							DrawType dType, bool onlyFixture)
{
	d2d::Colorf cFace, cEdge;
	if (onlyFixture)
		getFixtureColor(dType, cFace, cEdge);
	else
		getBodyColor(fixture->body->type, dType, cFace, cEdge);

	d2d::Matrix mt;
	fixture->body->sprite->GetTransMatrix(mt);
	fixture->draw(mt, cFace, cEdge);
}

void DrawUtils::getBodyColor(libmodeling::Body::Type type, DrawType dType,
							 d2d::Colorf& cFace, d2d::Colorf& cEdge)
{
	switch (type)
	{
	case libmodeling::Body::e_static:
		cFace.Set(0.5f, 0.9f, 0.5f, 0.2f);
		break;
	case libmodeling::Body::e_kinematic:
		cFace.Set(0.5f, 0.5f, 0.9f, 0.2f);
		break;
	case libmodeling::Body::e_dynamic:
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
			d2d::Colorf c = cFace;
			c.r *= rScale;
			c.g *= gScale;
			c.b *= bScale;
			cFace = colorEnlarge(c, 1.1f);
		}
		break;
	}

	cEdge = colorEnlarge(cFace, 1.05f);
}

void DrawUtils::getFixtureColor(DrawType type, d2d::Colorf& cFace, d2d::Colorf& cEdge)
{
	cFace.Set(0.6f, 0.35f, 0.6f, 0.5f);
	switch (type)
	{
	case e_mouseOn:
		cFace = colorEnlarge(cFace, 1.1f);
		break;
	case e_selected:
		cFace = colorEnlarge(cFace, 1.2f);
		break;
	}

	cEdge = colorEnlarge(cFace, 1.1f);
}

d2d::Colorf DrawUtils::colorEnlarge(const d2d::Colorf& color, float factor)
{
	d2d::Colorf ret = color;
	ret.r = std::min(color.r * factor, 1.0f);
	ret.g = std::min(color.g * factor, 1.0f);
	ret.b = std::min(color.b * factor, 1.0f);
	ret.a = color.a;

	return ret;
}
