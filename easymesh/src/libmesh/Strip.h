#ifndef _EASYMESH_STRIP_H_
#define _EASYMESH_STRIP_H_

#include "Shape.h"

#include <drag2d.h>

namespace emesh
{

class Node;
class Triangle;
class Strip : public Shape
{
public:
	Strip();
	Strip(const Strip& strip);
	Strip(const d2d::Image& image);

	//
	// ICloneable interface
	//
	virtual Strip* clone() const;

	//
	// IShape interface
	//
	virtual void Insert(const d2d::Vector& p);
	virtual void Remove(const d2d::Vector& p);
	virtual d2d::Vector* Find(const d2d::Vector& p);
	virtual void Move(d2d::Vector* src, const d2d::Vector& dst);

	virtual void Reset();
	virtual void Clear();

	virtual void ResetUVOffset(float dx, float dy);

private:
	void InitBound();
	
	void RefreshTriangles();

	void CopyTriangles(const Strip& strip);

	void AbsorbNodeToRegion(d2d::Vector& node);
	void RemoveCornerFromNodes();

	void InsertToEdge(std::vector<d2d::Vector>& edge, const d2d::Vector& p);

private:
	std::vector<d2d::Vector> m_left_nodes, m_right_nodes;

}; // Strip 

}

#endif // _EASYMESH_STRIP_H_