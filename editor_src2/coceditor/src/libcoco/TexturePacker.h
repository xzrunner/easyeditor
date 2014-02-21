#pragma once

#include <drag2d.h>

namespace libcoco
{

class TexturePacker
{
public:
	enum Type
	{
		e_png,
		e_ppm
	};

public:
	TexturePacker();

	void pack(const std::set<d2d::Image*>& images);

	void storeToMemory();
	void storeToFile(const std::string& floder, const std::string& filename, d2d::ImageSaver::Type type);

	const d2d::Rect* query(d2d::Image* image) const;

private:
	std::map<d2d::Image*, d2d::Rect> m_mapImg2Rect;
	
	int m_edge;

	int m_xCurr, m_yCurr, m_width;

	uint8_t* m_pixels;

}; // TexturePacker

class ImageCmp
{
public:
	enum Type
	{
		e_width = 0,
		e_height,
		e_area
	};

public:
	ImageCmp(Type type = e_width) : m_type(type) {}

	bool operator() (const d2d::Image* t0, const d2d::Image* t1) const 
	{
		switch (m_type)
		{
		case e_width:
			return t0->width() > t1->width();
		case e_height:
			return t0->height() > t1->height();
		case e_area:
			return t0->width() * t0->height() > t1->width() * t1->height();
		default:
			return t0->width() > t1->width();
		}
	}

private:
	Type m_type;

}; // ImageCmp

}