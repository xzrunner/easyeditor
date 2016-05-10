//#ifndef _EASYEDITOR_COLOR_H_
//#define _EASYEDITOR_COLOR_H_
//
//#include <iostream>
//
//namespace ee
//{
//
//class s2::Color
//{
//public:
//	float r, g, b, a;
//
//public:
//	s2::Color();
//	s2::Color(float r, float g, float b, float a = 1);
//
//	const s2::Color& operator = (const s2::Color& color);
//	bool operator == (const s2::Color& color) const;
//	bool operator != (const s2::Color& color) const;
//
//	void Valid();
//	void Set(float pr, float pg, float pb, float pa);
//	void Set(float pr, float pg, float pb);
//
//	unsigned int Pack() const;
//	void Unpack(unsigned int color);
//
//}; // s2::Color
//
//std::istream& operator >> (std::istream& is, s2::Color& color);
//std::ostream& operator << (std::ostream& os, const s2::Color& color);
//
//s2::Color col_mul(const s2::Color& a, const s2::Color& b);
//s2::Color col_add(const s2::Color& a, const s2::Color& b);
//s2::Color col_interpolate(const s2::Color& begin, const s2::Color& end, float scale);
//
//class Colori
//{
//public:
//	int r, g, b, a;
//
//public:
//	Colori();
//	Colori(int r, int g, int b, int a = 255);
//
//	void Set(int pr, int pg, int pb, int pa);
//
//	void Set(const Colori& c);
//
//}; // Colori
//
//}
//
//#include "Color.inl"
//
//#endif // _EASYEDITOR_COLOR_H_