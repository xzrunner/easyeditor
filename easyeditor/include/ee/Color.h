#ifndef _EASYEDITOR_COLOR_H_
#define _EASYEDITOR_COLOR_H_

#include <iostream>

namespace ee
{

class Colorf
{
public:
	float r, g, b, a;

public:
	Colorf();
	Colorf(float r, float g, float b, float a = 1);

	const Colorf& operator = (const Colorf& color);
	bool operator == (const Colorf& color) const;
	bool operator != (const Colorf& color) const;

	void Valid();
	void Set(float pr, float pg, float pb, float pa);
	void Set(float pr, float pg, float pb);

	unsigned int Pack() const;
	void Unpack(unsigned int color);

}; // Colorf

std::istream& operator >> (std::istream& is, Colorf& color);
std::ostream& operator << (std::ostream& os, const Colorf& color);

Colorf col_mul(const Colorf& a, const Colorf& b);
Colorf col_add(const Colorf& a, const Colorf& b);
Colorf col_interpolate(const Colorf& begin, const Colorf& end, float scale);

class Colori
{
public:
	int r, g, b, a;

public:
	Colori();
	Colori(int r, int g, int b, int a = 255);

	void Set(int pr, int pg, int pb, int pa);

	void Set(const Colori& c);

}; // Colori

}

#endif // _EASYEDITOR_COLOR_H_