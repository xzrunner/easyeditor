#ifndef _EASYSHAPE_PACKAGE_H_
#define _EASYSHAPE_PACKAGE_H_

namespace ee { class Shape; }

#include <fstream>

namespace eshape
{

class Paskage
{
public:
	enum {
		e_circle = 0,
		e_polygon
	};

public:
	static void PackShape(ee::Shape* shape, std::ofstream& fout);

}; // Paskage

}

#endif // _EASYSHAPE_PACKAGE_H_