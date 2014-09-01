#ifndef _LIBSHAPE_CHAIN_SHAPE_H_
#define _LIBSHAPE_CHAIN_SHAPE_H_

#include <drag2d.h>

namespace libshape
{

class ChainShape : public d2d::IShape
{
public:
	ChainShape();
	ChainShape(const ChainShape& chain);
	ChainShape(const std::vector<d2d::Vector>& vertices, bool isLoop);

	//
	// IObject interface
	//
	virtual ChainShape* clone() const;

	//
	// IShape interface
	//
	virtual bool isContain(const d2d::Vector& pos) const;
	virtual bool isIntersect(const d2d::Rect& rect) const;
	virtual const d2d::Rect& getRect() const { return m_rect; }
	virtual void draw(const d2d::Screen& scr, const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);

	virtual void insertVertices(size_t index, const d2d::Vector& pos);
	// todo: too slow! 
	// cache index or pointer? how to avoid iterator's invalid
	// maybe use list instead of array, but others' effective? 
	virtual void removeVertices(const d2d::Vector& pos);
	virtual void changeVertices(const d2d::Vector& from, const d2d::Vector& to);
	virtual void setVertices(const std::vector<d2d::Vector>& vertices);
	virtual void refresh() {}

	void setClosed(bool isClose) { m_isLoop = isClose; }
	bool isClosed() const { return m_isLoop; }

	const std::vector<d2d::Vector>& getVertices() const { return m_vertices; }

private:
	void initBounding();

protected:
	std::vector<d2d::Vector> m_vertices;
	bool m_isLoop;

	d2d::Rect m_rect;

}; // ChainShape

}

#endif // _LIBSHAPE_CHAIN_SHAPE_H_