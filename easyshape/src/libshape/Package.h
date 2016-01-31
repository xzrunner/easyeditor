#ifndef _LIBSHAPE_PACKAGE_H_
#define _LIBSHAPE_PACKAGE_H_



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
	static void packShape(ee::Shape* shape, std::ofstream& fout);

}; // Paskage

}

#endif // _LIBSHAPE_PACKAGE_H_