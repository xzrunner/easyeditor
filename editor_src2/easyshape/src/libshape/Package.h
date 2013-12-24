
#ifndef LIBSHAPE_PACKAGE_H
#define LIBSHAPE_PACKAGE_H

#include <drag2d.h>

namespace libshape
{
	class Paskage
	{
	public:
		enum {
			e_circle = 0,
			e_polygon
		};

	public:
		static void packShape(d2d::IShape* shape, std::ofstream& fout);

	}; // Paskage
}

#endif // LIBSHAPE_PACKAGE_H