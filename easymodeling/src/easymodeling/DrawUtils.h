#ifndef _EASYMODELING_DRAW_UTILS_H_
#define _EASYMODELING_DRAW_UTILS_H_

#include <easymodeling.h>

namespace emodeling
{

class Fixture;

class DrawUtils
{
public:
	enum DrawType
	{
		e_default,
		e_mouseOn,
		e_selected
	};

public:
	static void DrawBody(Body* body, DrawType dType);
	static void DrawFixture(Fixture* fixture, DrawType dType, bool onlyFixture);

private:
	static void GetBodyColor(Body::Type type, DrawType dType,
		s2::Color& cFace, s2::Color& cEdge);
	static void GetFixtureColor(DrawType type, s2::Color& cFace, 
		s2::Color& cEdge);

	static s2::Color ColorEnlarge(const s2::Color& color, float factor);

}; // DrawUtils

}

#endif // _EASYMODELING_DRAW_UTILS_H_
