#ifndef _EASYMESH_TRIANGLE_H_
#define _EASYMESH_TRIANGLE_H_

#include <CU_RefCountObj.h>

#include "Node.h"

namespace emesh
{

class Triangle : public cu::RefCountObj
{
public:
	Triangle() {}
	~Triangle() {
		for (int i = 0; i < 3; ++i) {
			nodes[i]->RemoveReference();
		}
	}
// 	Triangle(const Triangle& tri) {
// 		for (int i = 0; i < 3; ++i) {
// 			tri.nodes[0]->AddReference();
// 			nodes[0] = tri.nodes[0];
// 		}
// 	}

// 	Triangle(int width, int height, const sm::vec2& p0, 
// 		const sm::vec2& p1, const sm::vec2& p2) {
// 		nodes[0] = createNode(width, height, p0);
// 		nodes[1] = createNode(width, height, p1);
// 		nodes[2] = createNode(width, height, p2);
// 	}

private:
// 	Node createNode(int width, int height, const sm::vec2& p) {
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