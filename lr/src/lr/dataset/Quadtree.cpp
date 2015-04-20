#include "Quadtree.h"

#include <queue>

namespace lr
{

Quadtree::Quadtree(const d2d::Rect& rect)
{
	m_root = new Node(rect);
}

Quadtree::~Quadtree()
{
	delete m_root;
}

void Quadtree::Insert(const d2d::ISprite* spr)
{
	m_root->Insert(spr);
}

std::vector<const d2d::ISprite*> Quadtree::Query(const d2d::Rect& rect) const
{
	std::set<const d2d::ISprite*> set_sprites;

	std::queue<Node*> buffer;
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Node* node = buffer.front(); buffer.pop();
		if (node->IsLeaf()) {
			for (int i = 0, n = node->m_sprites.size(); i < n; ++i) {
				set_sprites.insert(node->m_sprites[i]);
			}
		} else {
			for (int i = 0; i < 4; ++i) {
				Node* cn = node->m_children[i];
				if (cn->IsIntersect(rect)) {
					buffer.push(cn);
				}
			}			
		}
	}

	std::vector<const d2d::ISprite*> sprites;
	copy(set_sprites.begin(), set_sprites.end(), back_inserter(sprites));
	return sprites;
}

//////////////////////////////////////////////////////////////////////////
// class Quadtree::Node
//////////////////////////////////////////////////////////////////////////

const int Quadtree::Node::MAX_COUNT = 5;

Quadtree::Node::
Node(const d2d::Rect& rect)
	: m_rect(rect)
{
	memset(m_children, 0, sizeof(m_children));
}

Quadtree::Node::
~Node()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
	for (int i = 0; i < 4; ++i) {
		delete m_children[i];
	}
}

void Quadtree::Node::
Insert(const d2d::ISprite* spr)
{
	spr->Retain();

	if (IsLeaf()) 
	{
	 	m_sprites.push_back(spr);
	 	if (m_sprites.size() > MAX_COUNT) {
	 		Split();
	 	}
	} 
	else 
	{
		d2d::Rect rect = GetSprRect(spr);
		for (int i = 0; i < 4; ++i) {
			Node* child = m_children[i];
			if (child->IsIntersect(rect)) {
				child->Insert(spr);
			}
		}
	}
}

bool Quadtree::Node::
IsLeaf() const
{
	return m_children[0] == NULL;
}

bool Quadtree::Node::
IsIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isRectIntersectRect(m_rect, rect);
}

void Quadtree::Node::
Split()
{
	for (int i = 0; i < 4; ++i) {
		m_children[i]->m_rect = m_rect;
	}
	m_children[0]->m_rect.xMax = m_children[2]->m_rect.xMax = 
		m_children[1]->m_rect.xMin = m_children[3]->m_rect.xMin = m_rect.xCenter();
	m_children[2]->m_rect.yMax = m_children[3]->m_rect.yMax = 
		m_children[0]->m_rect.yMin = m_children[1]->m_rect.yMin = m_rect.yCenter();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		const d2d::ISprite* spr = m_sprites[i];
		for (int j = 0; j < 4; ++j) {
			Node* node = m_children[j];
			d2d::Rect rect = GetSprRect(spr);
			if (node->IsIntersect(rect)) {
				node->Insert(spr);
			}
		}
	}

	m_sprites.clear();
}

d2d::Rect Quadtree::Node::
GetSprRect(const d2d::ISprite* spr)
{
	std::vector<d2d::Vector> bound;
	spr->getBounding()->getBoundPos(bound);
	d2d::Rect rect;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		rect.combine(bound[i]);
	}
	return rect;
}

}