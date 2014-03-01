
#ifndef ESHAPE_LOVE2D_CODE_H
#define ESHAPE_LOVE2D_CODE_H

#include <easybuilder.h>

namespace eshape
{
	class LibraryItem;

	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen);

		void resolve();

	private:
		void resolve(LibraryItem* item);
		void resolve(d2d::IShape* shape);
		void resolve(const d2d::BezierShape* bezier);
		void resolve(const d2d::PolygonShape* poly);
		void resolve(const d2d::ChainShape* chain);
		void resolve(const d2d::RectShape* rect);
		void resolve(const d2d::CircleShape* circle);

	private:
		ebuilder::CodeGenerator& m_gen;

	}; // Love2dCode
}

#endif // ESHAPE_LOVE2D_CODE_H