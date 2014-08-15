#ifndef _EASYMESH_ISHAPE_H_
#define _EASYMESH_ISHAPE_H_

#include <drag2d.h>

#include "config.h"

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

	virtual void OffsetUV(float dx, float dy) = 0;

	virtual void Load(const Json::Value& value) = 0;
	virtual void Store(Json::Value& value) const = 0;

	virtual int GetQuadSize() const = 0;

	virtual ShapeType GetShapeType() const  = 0;

	void QueryNode(const d2d::Vector& p, std::vector<Node*>& nodes);
	void QueryNode(const d2d::Rect& r, std::vector<Node*>& nodes);

	void DrawInfoUV() const;
	void DrawInfoXY() const;
	void DrawTexture() const;

	const std::vector<Triangle*>& GetTriangles() const { return m_tris; }

	float GetNodeRegion() const { return m_node_radius; }

	d2d::Rect GetRegion() const;

	void SetTween(Shape* begin, Shape* end, float process);

protected:
	void ClearTriangles();

	void StoreTriangles(Json::Value& value) const;
	void LoadTriangles(const Json::Value& value);

protected:
	int m_texid;
	float m_width, m_height;

	std::vector<Triangle*> m_tris;

	float m_node_radius;

}; // IShape

}

#endif // _EASYMESH_ISHAPE_H_