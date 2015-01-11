#ifndef _EASYMESH_SHAPE_H_
#define _EASYMESH_SHAPE_H_

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

	virtual void Load(const Json::Value& value) {}
	virtual void Store(Json::Value& value) const {}

	virtual void OffsetUV(float dx, float dy) {}
	virtual void Update() {}
	virtual void Refresh() {}

	void QueryNode(const d2d::Vector& p, std::vector<Node*>& nodes);
	void QueryNode(const d2d::Rect& r, std::vector<Node*>& nodes);

	void DrawInfoUV() const;
	void DrawInfoXY() const;
	void DrawTexture(const d2d::Matrix& mt) const;

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

}; // Shape

}

#endif // _EASYMESH_SHAPE_H_