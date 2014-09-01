#ifndef _DRAG2D_RENDER_LIST_H_
#define _DRAG2D_RENDER_LIST_H_

#include "common/Rect.h"
#include "dataset/Image.h"

#include <set>
#include <vector>

namespace d2d
{

class RenderList
{
public:
	static RenderList* Instance();

	void Insert(int tex, Vector vertices[4], Vector texcoords[4]);

//	void Draw() const;

	void Flush();

private:
	RenderList() {}
	~RenderList() {}

private:
	struct Node
	{
		int tex;
		Vector vertices[4], texcoords[4];
	}; // Node

private:
	std::vector<Node> m_nodes;

private:
	static RenderList* m_instance;

}; // RenderList

}

#endif // _DRAG2D_RENDER_LIST_H_