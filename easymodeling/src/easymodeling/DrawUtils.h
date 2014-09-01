
#ifndef EMODELING_DRAW_UTILS_H
#define EMODELING_DRAW_UTILS_H

#include <drag2d.h>

#include <easymodeling.h>

namespace emodeling
{
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
		static void drawBody(const d2d::Screen& scr, libmodeling::Body* body, DrawType dType);
		static void drawFixture(const d2d::Screen& scr, libmodeling::Fixture* fixture, 
			DrawType dType, bool onlyFixture);

	private:
		static void getBodyColor(libmodeling::Body::Type type, DrawType dType,
			d2d::Colorf& cFace, d2d::Colorf& cEdge);
		static void getFixtureColor(DrawType type, d2d::Colorf& cFace, 
			d2d::Colorf& cEdge);

		static d2d::Colorf colorEnlarge(const d2d::Colorf& color, float factor);

	}; // DrawUtils
}

#endif // EMODELING_DRAW_UTILS_H
