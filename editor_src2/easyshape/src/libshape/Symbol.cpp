#include "Symbol.h"
#include "FileAdapter.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace libshape
{

const float Symbol::SCALE = 0.5f;

Symbol::Symbol()
{
}

Symbol::Symbol(const Symbol& symbol)
{
	m_filepath = symbol.m_filepath;
	shapes.reserve(symbol.shapes.size());
	for (size_t i = 0, n = symbol.shapes.size(); i < n; ++i)
		shapes.push_back(symbol.shapes[i]->clone());
}

Symbol::~Symbol()
{
	clear();
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this); 
}

void Symbol::reloadTexture() const
{
}

void Symbol::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
	const d2d::ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->draw();
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	d2d::Rect rect;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		rect.combine(shapes[i]->getRect());
	return rect;
}

void Symbol::refresh()
{
	loadThumbnail();
}

void Symbol::loadResources()
{
	clear();

	FileAdapter adpater(shapes);
	adpater.load(m_filepath.c_str());

	loadThumbnail();
}

void Symbol::loadThumbnail()
{
	if (shapes.empty()) return;

	static const int PADDING = 2;

	d2d::Rect rect;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		rect.combine(shapes[i]->getRect());

	//if (m_bitmap) delete m_bitmap;
	//m_bitmap = new Bitmap(
	//	new wxBitmap(
	//	rect.xLength() * SCALE + PADDING, 
	//	rect.yLength() * SCALE + PADDING)
	//	);

	const float dx = (-rect.xMin * SCALE) + PADDING*0.5f,
		dy = (rect.yMax * SCALE) + PADDING*0.5f;

	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.Clear();

	for (size_t i = 0, n = shapes.size(); i < n ;++i)
	{
		d2d::IShape* shape = shapes[i];
		if (BezierShape* bezier = dynamic_cast<BezierShape*>(shape))
		{
			size_t size = bezier->getVertices().size();
			wxPoint* points = new wxPoint[size];
			for (size_t i = 0; i < size; ++i)
			{
				points[i].x = bezier->getVertices()[i].x * SCALE + dx;
				points[i].y = -bezier->getVertices()[i].y * SCALE + dy;
			}
			memDC.DrawLines(size, points);
			delete points;
		}
		else if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape))
		{
			size_t size = poly->getVertices().size();
			wxPoint* points = new wxPoint[size];
			for (size_t i = 0; i < size; ++i)
			{
				points[i].x = poly->getVertices()[i].x * SCALE + dx;
				points[i].y = -poly->getVertices()[i].y * SCALE + dy;
			}
			memDC.DrawPolygon(size, points);
			delete points;
		}
		else if (ChainShape* chain = dynamic_cast<ChainShape*>(shape))
		{
			size_t size = chain->getVertices().size();
			wxPoint* points = new wxPoint[size];
			for (size_t i = 0; i < size; ++i)
			{
				points[i].x = chain->getVertices()[i].x * SCALE + dx;
				points[i].y = -chain->getVertices()[i].y * SCALE + dy;
			}
			memDC.DrawLines(size, points);
			delete points;
		}
		else if (RectShape* rect = dynamic_cast<RectShape*>(shape))
		{
			memDC.DrawRectangle(
				rect->m_rect.xMin * SCALE + dx,
				-rect->m_rect.yMax * SCALE + dy,
				rect->m_rect.xLength() * SCALE,
				rect->m_rect.yLength() * SCALE);
		}
		else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
		{
			memDC.DrawCircle(wxPoint(circle->center.x * SCALE + dx, -circle->center.y * SCALE + dy), 
				circle->radius * SCALE);
		}
	}

	memDC.SelectObject(wxNullBitmap);
}

void Symbol::clear()
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->release();
	shapes.clear();
}

}