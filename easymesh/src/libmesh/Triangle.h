#ifndef _EASYMESH_TRIANGLE_H_
#define _EASYMESH_TRIANGLE_H_

#include "Node.h"

namespace emesh
{

class Triangle : public d2d::Object
{
public:
	Triangle() {}
// 	Triangle(const Triangle& tri) {
// 		for (int i = 0; i < 3; ++i) {
// 			tri.nodes[0]->Retain();
// 			nodes[0] = tri.nodes[0];
// 		}
// 	}

// 	Triangle(int width, int height, const d2d::Vector& p0, 
// 		const d2d::Vector& p1, const d2d::Vector& p2) {
// 		nodes[0] = createNode(width, height, p0);
// 		nodes[1] = createNode(width, height, p1);
// 		nodes[2] = createNode(width, height, p2);
// 	}

private:
// 	Node createNode(int width, int height, const d2d::Vector& p) {
// 		Node n;
// 		n.src.x = p.x / width + 0.5f;
// 		n.src.y = p.y / height + 0.5f;
// 		n.dst = p;
// 		return n;
// 	}

public:
	Node* nodes[3];

}; // Triangle

}

#endif // _EASYMESH_TRIANGLE_H_