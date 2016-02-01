#ifndef _EASYSHAPE_PACKAGE_H_
#define _EASYSHAPE_PACKAGE_H_



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
	static void packShape(ee::Shape* shape, std::ofstream& fout);

}; // Paskage

}

#endif // _EASYSHAPE_PACKAGE_H_