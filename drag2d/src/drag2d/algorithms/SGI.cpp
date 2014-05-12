#include "SGI.h"

#include <assert.h>
#include <set>

#include "common/tools.h"

namespace d2d
{

void SGI::implement(const std::vector<Vector>& src, 
					std::vector<std::vector<Vector> >& dst)
{
	if (src.size() < 3) return;

	std::vector<Triangle*> tris;
	std::vector<Edge*> edges;
	int index = 0;
	for (size_t i = 0, n = src.size() / 3; i < n; ++i)
	{
		Triangle* tri = new Triangle;
		for (size_t j = 0; j < 3; ++j)
			tri->nodes[j] = src[index++];
		for (size_t j = 0; j < 3; ++j)
			insertEdge(edges, tri, j);
		tris.push_back(tri);
	}

	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		if (tris[i]->bUsed) continue;

		std::vector<Vector> strip;
		traversal(tris[i], strip);
		dst.push_back(strip);
	}

	for_each(tris.begin(), tris.end(), DeletePointerFunctor<Triangle>());
	for_each(edges.begin(), edges.end(), DeletePointerFunctor<Edge>());
}

void SGI::insertEdge(std::vector<Edge*>& edges, Triangle* tri,
					 int index)
{
	bool bFind = false;
	const Vector& s = tri->nodes[index],
		e = tri->nodes[(index+1)%3];
	for (size_t i = 0, n = edges.size(); i < n; ++i)
	{
		Edge* edge = edges[i];
		if (edge->start == s && edge->end == e)
		{
			assert(!edge->left);
			edge->left = tri;
			tri->edges[index] = edge;
			bFind = true;
			break;
		}
		else if (edge->start == e && edge->end == s)
		{
			assert(!edge->right);
			edge->right = tri;
			tri->edges[index] = edge;
			bFind = true;
			break;
		}
	}

	if (!bFind)
	{
		Edge* edge = new Edge;
		edge->start = s;
		edge->end = e;
		edge->left = tri;
		edges.push_back(edge);
		tri->edges[index] = edge;
	}
}

void SGI::traversal(Triangle* tri, std::vector<Vector>& strip, int level/* = 0*/)
{
	tri->bUsed = true;

	std::vector<Triangle*> tris;
	for (size_t i = 0; i < 3; ++i)
	{
		Edge* edge = tri->edges[i];
		if (edge->left && edge->left != tri && !edge->left->bUsed)
			tris.push_back(edge->left);
		else if (edge->right && edge->right != tri && !edge->right->bUsed)
			tris.push_back(edge->right);
	}

	if (tris.empty())
	{
		if (level == 0)
		{
			for (size_t i = 0; i < 3; ++i)
				strip.push_back(tri->nodes[i]);
		}
	}
	else
	{
		Triangle* next = getMinDegreeTri(tris);

		if (level == 0)
		{
			if (tri->edges[0]->left == next || tri->edges[0]->right == next)
			{
				strip.push_back(tri->nodes[2]);
				strip.push_back(tri->nodes[0]);
				strip.push_back(tri->nodes[1]);
			}
			else if (tri->edges[1]->left == next || tri->edges[1]->right == next)
			{
				strip.push_back(tri->nodes[0]);
				strip.push_back(tri->nodes[1]);
				strip.push_back(tri->nodes[2]);
			}
			else if (tri->edges[2]->left == next || tri->edges[2]->right == next)
			{
				strip.push_back(tri->nodes[1]);
				strip.push_back(tri->nodes[2]);
				strip.push_back(tri->nodes[0]);
			}
			else 
				assert(0);
		}

		if (next->edges[0]->left == tri || next->edges[0]->right == tri)
			strip.push_back(next->nodes[2]);
		else if (next->edges[1]->left == tri || next->edges[1]->right == tri)
			strip.push_back(next->nodes[0]);
		else if (next->edges[2]->left == tri || next->edges[2]->right == tri)
			strip.push_back(next->nodes[1]);
		else 
			assert(0);

		traversal(next, strip, level + 1);
	}
}

int SGI::getDegree(Triangle* tri, int level)
{
	std::vector<Triangle*> buffer;
	buffer.push_back(tri);
	for (size_t i = 0; i < level; ++i)
	{
		std::set<Triangle*> nextLevelTris;
		for (size_t j = 0, m = buffer.size(); j < m; ++j)
		{
			Triangle* tri = buffer[j];
			for (size_t k = 0; k < 3; ++k)
			{
				Edge* edge = tri->edges[k];
				if (edge->left && edge->left != tri && !edge->left->bUsed)
					nextLevelTris.insert(edge->left);
				else if (edge->right && edge->right != tri && !edge->right->bUsed)
					nextLevelTris.insert(edge->right);
			}
		}

		buffer.clear();
		buffer.reserve(nextLevelTris.size());
		std::set<Triangle*>::iterator itr = nextLevelTris.begin();
		for ( ; itr != nextLevelTris.end(); ++itr)
			buffer.push_back(*itr);
	}

	int degree = 0;
	for (size_t i = 0, n = buffer.size(); i < n; ++i)
	{
		Triangle* tri = buffer[i];
		for (size_t j = 0; j < 3; ++j)
		{
			Edge* edge = tri->edges[j];
			if (edge->left && edge->left != tri && !edge->left->bUsed)
				++degree;
			else if (edge->right && edge->right != tri && !edge->right->bUsed)
				++degree;
		}
	}

	return degree;
}

SGI::Triangle* SGI::getMinDegreeTri(const std::vector<Triangle*>& tris)
{
	if (tris.empty()) return NULL;
	if (tris.size() == 1) return tris[0];

	int level = 0;
	while (level < 10)
	{
		std::vector<int> degrees;
		degrees.reserve(tris.size());
		for (size_t i = 0, n = tris.size(); i < n; ++i)
			degrees.push_back(getDegree(tris[i], level));

		for (size_t i = 0, n = tris.size(); i < n; ++i)
		{
			size_t j;
			for (j = 0; j < n; ++j)
				if (i != j && tris[j] <= tris[i])
					break;
			if (j == n)
				return tris[i];
		}
		
		++level;
	}

	return NULL;
}

} // d2d