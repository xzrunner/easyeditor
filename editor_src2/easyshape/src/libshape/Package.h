#ifndef _LIBSHAPE_PACKAGE_H_
#define _LIBSHAPE_PACKAGE_H_

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

#endif // _LIBSHAPE_PACKAGE_H_