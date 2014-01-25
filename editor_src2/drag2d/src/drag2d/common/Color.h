#pragma once

#include <iostream>

namespace d2d
{
	struct Colorf
	{
		float r, g, b, a;

		Colorf() 
			: r(0), g(0), b(0), a(1) {}
		Colorf(float _r, float _g, float _b) 
			: r(_r), g(_g), b(_b), a(1) {}
		Colorf(float _r, float _g, float _b, float _a) 
			: r(_r), g(_g), b(_b), a(_a) {}

		const Colorf& operator = (const Colorf& color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
			return *this;
		}

		bool operator != (const Colorf& color) const
		{
			return r != color.r || g != color.g || b != color.b || a != color.a;
		}

		void valid() {
			r = std::max(0.0f, std::min(r, 1.0f));
			g = std::max(0.0f, std::min(g, 1.0f));
			b = std::max(0.0f, std::min(b, 1.0f));
			a = std::max(0.0f, std::min(a, 1.0f));
		}

		void set(float pr, float pg, float pb, float pa) {
			r = pr;
			g = pg;
			b = pb;
			a = pa;
		}

		void set(float pr, float pg, float pb) {
			r = pr;
			g = pg;
			b = pb;
		}

		unsigned int pack() const {
			return ((int)(r * 255 + 0.5f) << 24) | ((int)(g * 255 + 0.5f) << 16) | ((int)(b * 255 + 0.5f) << 8) | ((int)(r * 255 + 0.5f));
		}
		void unpack(unsigned int color) {
			r = ((color & 0xff000000) >> 24) / 255.0f;
			g = ((color & 0x00ff0000) >> 16) / 255.0f;
			b = ((color & 0x0000ff00) >> 8) / 255.0f;
			a = ((color & 0x000000ff)) / 255.0f;
		}

	}; // Colorf

	inline std::istream& operator >> (std::istream& is, Colorf& color)
	{
		is >> color.r >> color.g >> color.b >> color.a;
		return is;
	}
	inline std::ostream& operator << (std::ostream& os, const Colorf& color)
	{
		os << color.r << " " << color.g << " " << color.b << " " << color.a;
		return os;
	}

	inline Colorf cMul(const Colorf& a, const Colorf& b)
	{
		Colorf ret;
		ret.r = a.r * b.r;
		ret.g = a.g * b.g;
		ret.b = a.b * b.b;
		ret.a = a.a * b.a;
		return ret;
	}

	inline Colorf cAdd(const Colorf& a, const Colorf& b)
	{
		Colorf ret;
		ret.r = std::min(a.r + b.r, 1.0f);
		ret.g = std::min(a.g + b.g, 1.0f);
		ret.b = std::min(a.b + b.b, 1.0f);
		ret.a = std::min(a.a + b.a, 1.0f);
		return ret;
	}

	inline Colorf cInterpolate(const Colorf& begin, const Colorf& end, float scale)
	{
		Colorf ret;
		ret.r = begin.r + (end.r - begin.r) * scale;
		ret.g = begin.g + (end.g - begin.g) * scale;
		ret.b = begin.b + (end.b - begin.b) * scale;
		ret.a = begin.a + (end.a - begin.a) * scale;
		return ret;
	}

	struct Colori
	{
		int r, g, b, a;

		Colori()
			: r(0), g(0), b(0), a(255) {}
		Colori(int _r, int _g, int _b, int _a) 
			: r(_r), g(_g), b(_b), a(_a) {}

		void set(int pr, int pg, int pb, int pa) {
			r = pr;
			g = pg;
			b = pb;
			a = pa;
		}

		void set(const Colori& c) { set(c.r, c.g, c.b, c.a); }

	}; // Colori
}

