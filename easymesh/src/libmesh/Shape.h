#ifndef _EASYMESH_ISHAPE_H_
#define _EASYMESH_ISHAPE_H_

#include <drag2d.h>

namespace emesh
{

class Node;
class Triangle;

class Shape : public d2d::Object, public d2d::ICloneable
{
public:
	Shape();
	Shape(const Shape& shape);
	Shape(const d2d::Image& image);
	virtual ~Shape();

 	//
 	// ICloneable interface
 	//
 	virtual Shape* clone() const { return NULL; }

	virtual void Insert(const d2d::Vector& p) = 0;
	virtual void Remove(const d2d::Vector& p) = 0;
	virtual d2d::Vector* Find(const d2d::Vector& p) = 0;
	virtual void Move(d2d::Vector* src, const d2d::Vector& dst) = 0;

	virtual void Reset() = 0;
	virtual void Clear() = 0;

	virtual void ResetUVOffset(float dx, float dy) = 0;

	Node* QueryNode(const d2d::Vector& p);
	void QueryNode(const d2d::Rect& r, std::vector<Node*>& nodes);

	void DrawInfoUV() const;
	void DrawInfoXY() const;
	void DrawTexture() const;

protected:
	void ClearTriangles();

protected:
	int m_texid;
	float m_width, m_height;

	std::vector<Triangle*> m_tris;

}; // IShape

}

#endif // _EASYMESH_ISHAPE_H_