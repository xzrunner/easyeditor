
#ifndef EMODELING_DRAW_UTILS_H
#define EMODELING_DRAW_UTILS_H



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
		static void drawBody(libmodeling::Body* body, DrawType dType);
		static void drawFixture(libmodeling::Fixture* fixture, 
			DrawType dType, bool onlyFixture);

	private:
		static void getBodyColor(libmodeling::Body::Type type, DrawType dType,
			ee::Colorf& cFace, ee::Colorf& cEdge);
		static void getFixtureColor(DrawType type, ee::Colorf& cFace, 
			ee::Colorf& cEdge);

		static ee::Colorf colorEnlarge(const ee::Colorf& color, float factor);

	}; // DrawUtils
}

#endif // EMODELING_DRAW_UTILS_H
