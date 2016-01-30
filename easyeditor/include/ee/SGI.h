#ifndef _EASYEDITOR_SGI_H_
#define _EASYEDITOR_SGI_H_

#include "Vector.h"

#include <vector>

namespace ee
{

class SGI
{
public:
	static void Do(const std::vector<Vector>& src,
		std::vector<std::vector<Vector> >& dst);

private:
	struct Edge;

	struct Triangle
	{
		bool used;

		Vector nodes[3];

		// [0] v0-v1, [1] v1-v2, [2] v2-v0
		Edge* edges[3];

		Triangle() {
			used = false;
			edges[0] = edges[1] = edges[2] = NULL;
		}
	};

	struct Edge
	{
		Vector start, end;

		Triangle *left, *right;

		Edge() {
			left = right = NULL;
		}
	};

private:
	static void InsertEdge(std::vector<Edge*>& edges, Triangle* tri,
		int index);

	static void Traversal(Triangle* tri, std::vector<Vector>& strip, int level = 0);

	static int GetDegree(Triangle* tri, int level);

	static Triangle* GetMinDegreeTri(const std::vector<Triangle*>& tris);

}; // SGI

}

#endif // _EASYEDITOR_SGI_H_