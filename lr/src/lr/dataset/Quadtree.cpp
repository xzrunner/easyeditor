#include "Quadtree.h"

#include <queue>

namespace lr
{

Quadtree::Quadtree(const d2d::Rect& rect)
	: m_selected(NULL)
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

void Quadtree::DebugDraw() const
{
	std::queue<Node*> buffer;
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Node* node = buffer.front(); buffer.pop();

		d2d::PrimitiveDraw::rect(d2d::Matrix(), node->m_rect, d2d::LIGHT_GREY_THIN_LINE);

		if (!node->IsLeaf()) 
		{
			for (int i = 0; i < 4; ++i) {
				Node* cn = node->m_children[i];
				buffer.push(cn);
			}			
		}
	}

	if (m_selected) {
		d2d::PrimitiveDraw::rect(d2d::Matrix(), m_selected->m_rect, d2d::LIGHT_RED_THIN_LINE);
		for (int i = 0, n = m_selected->m_sprites.size(); i < n; ++i) {
			d2d::SpriteDraw::drawSprite(m_selected->m_sprites[i], d2d::Matrix(), d2d::LIGHT_BLUE);
		}
	}
}

void Quadtree::SelectNode(const d2d::Vector& pos)
{
	std::queue<Node*> buffer;
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Node* node = buffer.front(); buffer.pop();
		if (node->IsLeaf()) {
			m_selected = node;
		} else {
			for (int i = 0; i < 4; ++i) {
				Node* cn = node->m_children[i];
				if (cn->IsContain(pos)) {
					buffer.push(cn);
				}
			}			
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class Quadtree::Node
//////////////////////////////////////////////////////////////////////////

const int Quadtree::Node::MAX_COUNT = 5;
const float Quadtree::Node::MAX_AREA = 0.5f;
const int Quadtree::Node::MIN_GRID = 8;

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
	 	if (NeedSplit()) {
	 		Split();
	 	}
	} 
	else 
	{
		d2d::Rect rect = spr->GetRect();
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

bool Quadtree::Node::
IsContain(const d2d::Vector& pos) const
{
	return d2d::Math::isPointInRect(pos, m_rect);
}

bool Quadtree::Node::
IsContain(const d2d::ISprite* spr) const
{
	
}

bool Quadtree::Node::
NeedSplit() const
{
	return m_sprites.size() > MAX_COUNT;
}

void Quadtree::Node::
Split()
{
	for (int i = 0; i < 4; ++i) {
		m_children[i] = new Node(m_rect);
	}
	m_children[0]->m_rect.xMax = m_children[2]->m_rect.xMax = 
		m_children[1]->m_rect.xMin = m_children[3]->m_rect.xMin = m_rect.xCenter();
	m_children[2]->m_rect.yMax = m_children[3]->m_rect.yMax = 
		m_children[0]->m_rect.yMin = m_children[1]->m_rect.yMin = m_rect.yCenter();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		const d2d::ISprite* spr = m_sprites[i];
		for (int j = 0; j < 4; ++j) {
			Node* node = m_children[j];
			d2d::Rect rect = spr->GetRect();
			if (node->IsIntersect(rect)) {
				node->Insert(spr);
			}
		}
	}

	m_sprites.clear();
}

}