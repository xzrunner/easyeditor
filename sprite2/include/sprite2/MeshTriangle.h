#ifndef _SPRITE2_MESH_TRIANGLE_H_
#define _SPRITE2_MESH_TRIANGLE_H_

#include "MeshNode.h"

namespace s2
{

class MeshNode;

class MeshTriangle : public cu::RefCountObj
{
public:
	MeshTriangle() {}
	~MeshTriangle() {
		for (int i = 0; i < 3; ++i) {
			nodes[i]->RemoveReference();
		}
	}
	// 	MeshTriangle(const MeshTriangle& tri) {
	// 		for (int i = 0; i < 3; ++i) {
	// 			tri.nodes[0]->AddReference();
	// 			nodes[0] = tri.nodes[0];
	// 		}
	// 	}

	// 	MeshTriangle(int width, int height, const sm::vec2& p0, 
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
	MeshNode* nodes[3];

}; // MeshTriangle

}

#endif // _SPRITE2_MESH_TRIANGLE_H_