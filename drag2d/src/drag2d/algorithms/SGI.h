#pragma once

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class SGI
	{
	public:

		static void implement(const std::vector<Vector>& src,
			std::vector<std::vector<Vector> >& dst);

	private:
		struct Edge;

		struct Triangle
		{
			bool bUsed;

			Vector nodes[3];

			// [0] v0-v1, [1] v1-v2, [2] v2-v0
			Edge* edges[3];

			Triangle() {
				bUsed = false;
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
		static void insertEdge(std::vector<Edge*>& edges, Triangle* tri,
			int index);

		static void traversal(Triangle* tri, std::vector<Vector>& strip, int level = 0);

		static int getDegree(Triangle* tri, int level);

		static Triangle* getMinDegreeTri(const std::vector<Triangle*>& tris);

	}; // SGI
}

