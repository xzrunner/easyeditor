
#ifndef ESHAPE_LOVE2D_CODE_H
#define ESHAPE_LOVE2D_CODE_H

#include <easybuilder.h>
#include <easyshape.h>

namespace eshape
{
	class LibraryPanel;
	class LibraryItem;

	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen, LibraryPanel* library);

		void resolve();

	private:
		void resolve(LibraryItem* item);
		void resolve(d2d::IShape* shape);
		void resolve(const libshape::BezierShape* bezier);
		void resolve(const libshape::PolygonShape* poly);
		void resolve(const libshape::ChainShape* chain);
		void resolve(const libshape::RectShape* rect);
		void resolve(const libshape::CircleShape* circle);

	private:
		ebuilder::CodeGenerator& m_gen;

		LibraryPanel* m_library;

	}; // Love2dCode
}

#endif // ESHAPE_LOVE2D_CODE_H