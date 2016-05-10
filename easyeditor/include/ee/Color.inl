//#ifndef _EASYEDITOR_COLOR_INL_
//#define _EASYEDITOR_COLOR_INL_
//
//namespace ee
//{
//
//inline
//s2::Color::s2::Color() 
//	: r(0), g(0), b(0), a(1) 
//{}
//
//inline
//s2::Color::s2::Color(float _r, float _g, float _b, float _a) 
//	: r(_r), g(_g), b(_b), a(_a) 
//{}
//
//inline
//const s2::Color& s2::Color::operator = (const s2::Color& color)
//{
//	r = color.r;
//	g = color.g;
//	b = color.b;
//	a = color.a;
//	return *this;
//}
//
//inline
//bool s2::Color::operator == (const s2::Color& color) const
//{
//	return r == color.r && g == color.g && b == color.b && a == color.a;
//}
//
//inline
//bool s2::Color::operator != (const s2::Color& color) const
//{
//	return r != color.r || g != color.g || b != color.b || a != color.a;
//}
//
//inline
//void s2::Color::Valid() 
//{
//	r = (std::max)(0.0f, (std::min)(r, 1.0f));
//	g = (std::max)(0.0f, (std::min)(g, 1.0f));
//	b = (std::max)(0.0f, (std::min)(b, 1.0f));
//	a = (std::max)(0.0f, (std::min)(a, 1.0f));
//}
//
//inline
//void s2::Color::Set(float pr, float pg, float pb, float pa) 
//{
//	r = pr;
//	g = pg;
//	b = pb;
//	a = pa;
//}
//
//inline
//void s2::Color::Set(float pr, float pg, float pb) 
//{
//	r = pr;
//	g = pg;
//	b = pb;
//}
//
//inline
//unsigned int s2::Color::Pack() const 
//{
//	return ((int)(r * 255 + 0.5f) << 24) | ((int)(g * 255 + 0.5f) << 16) | ((int)(b * 255 + 0.5f) << 8) | ((int)(a * 255 + 0.5f));
//}
//
//inline
//void s2::Color::Unpack(unsigned int color) 
//{
//	r = ((color & 0xff000000) >> 24) / 255.0f;
//	g = ((color & 0x00ff0000) >> 16) / 255.0f;
//	b = ((color & 0x0000ff00) >> 8) / 255.0f;
//	a = ((color & 0x000000ff)) / 255.0f;
//}
//
//inline 
//std::istream& operator >> (std::istream& is, s2::Color& color)
//{
//	is >> color.r >> color.g >> color.b >> color.a;
//	return is;
//}
//
//inline 
//std::ostream& operator << (std::ostream& os, const s2::Color& color)
//{
//	os << color.r << " " << color.g << " " << color.b << " " << color.a;
//	return os;
//}
//
//inline 
//s2::Color col_mul(const s2::Color& a, const s2::Color& b)
//{
//	s2::Color ret;
//	ret.r = a.r * b.r;
//	ret.g = a.g * b.g;
//	ret.b = a.b * b.b;
//	ret.a = a.a * b.a;
//	return ret;
//}
//
//inline 
//s2::Color col_add(const s2::Color& a, const s2::Color& b)
//{
//	s2::Color ret;
//	ret.r = (std::min)(a.r + b.r, 1.0f);
//	ret.g = (std::min)(a.g + b.g, 1.0f);
//	ret.b = (std::min)(a.b + b.b, 1.0f);
//	ret.a = (std::min)(a.a + b.a, 1.0f);
//	return ret;
//}
//
//inline 
//s2::Color col_interpolate(const s2::Color& begin, const s2::Color& end, float scale)
//{
//	s2::Color ret;
//	ret.r = begin.r + (end.r - begin.r) * scale;
//	ret.g = begin.g + (end.g - begin.g) * scale;
//	ret.b = begin.b + (end.b - begin.b) * scale;
//	ret.a = begin.a + (end.a - begin.a) * scale;
//	return ret;
//}
//
//inline
//Colori::Colori()
//	: r(0), g(0), b(0), a(255) 
//{}
//
//inline
//Colori::Colori(int _r, int _g, int _b, int _a) 
//	: r(_r), g(_g), b(_b), a(_a) 
//{}
//
//inline
//void Colori::Set(int pr, int pg, int pb, int pa) 
//{
//	r = pr;
//	g = pg;
//	b = pb;
//	a = pa;
//}
//
//inline
//void Colori::Set(const Colori& c) 
//{ 
//	Set(c.r, c.g, c.b, c.a); 
//}
//
//}
//
//#endif // _EASYEDITOR_COLOR_INL_
