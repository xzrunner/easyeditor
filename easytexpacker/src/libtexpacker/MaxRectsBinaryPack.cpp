#include "MaxRectsBinaryPack.h"


#include <algorithm>
#include <math.h>

namespace libtexpacker
{

static const float SCALE = 1.0f;
static const int PADDING = 0;

static const int REINSERT_MAX = 128;

MaxRectsBinaryPack::MaxRectsBinaryPack()
	: m_root(NULL)
{
}

MaxRectsBinaryPack::~MaxRectsBinaryPack()
{
	delete m_root;
}

RectSize MaxRectsBinaryPack::GetSize() const
{
	RectSize sz;
	if (m_root) {
		sz.width = m_root->Width();
		sz.height = m_root->Height();
	}
	return sz;
}

void MaxRectsBinaryPack::Pack(const std::vector<RectSize>& rects, std::vector<Rect>& output)
{
	if (rects.empty()) {
		return;
	}

	// prepare data
	int sz = rects.size();
	output.clear();
	output.resize(sz);

	std::vector<Sprite> sprites;
	sprites.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		sprites.push_back(Sprite(&rects[i], &output[i]));
	}
	std::sort(sprites.begin(), sprites.end(), SpriteCmp(e_area));

	// compute base size
	float area = 0;
	for (int i = 0; i < sz; ++i) {
		area += rects[i].width * rects[i].height;
	}
	int edge = next_p2((int)ceil(sqrt(area)));

	// insert
	bool success = false;
	while (!success) 
	{
		int max_arranged = -1, max_flag = 0;

		int flag = 0;
		while (flag < REINSERT_MAX)
		{
			ResetRoot(edge, edge);

			int i = 0, n = sprites.size();
			for ( ; i < n; ++i) {
				Node* result = m_root->insert(&sprites[i], flag);
				if (!result) {
					++flag;
					break;
				}
			}

			if (i == n) {
				success = true;
				break;
			} else if (i > max_arranged) {
				max_arranged = i;
				max_flag = flag - 1;
			}
		}

		edge *= 2;
	}
}

void MaxRectsBinaryPack::ResetRoot(int width, int height)
{
	delete m_root;
	m_root = new Node(width, height);
}

//////////////////////////////////////////////////////////////////////////
// class MaxRectsBinaryPack::Node
//////////////////////////////////////////////////////////////////////////

MaxRectsBinaryPack::Node::
Node()
	: m_sprite(NULL)
{
	m_child[0] = m_child[1] = NULL;	
}

MaxRectsBinaryPack::Node::
Node(int width, int height)
	: m_sprite(NULL)
{
	m_child[0] = m_child[1] = NULL;
	m_rc.x = m_rc.y = 0;
	m_rc.width = width;
	m_rc.height = height;
}

MaxRectsBinaryPack::Node::
~Node()
{
	delete m_child[0];
	delete m_child[1];
}

MaxRectsBinaryPack::Node* MaxRectsBinaryPack::Node::
insert(Sprite* sprite, int flag)
{
	if (m_child[0])
	{
		Node* new_node = m_child[0]->insert(sprite, flag / 2);
		if (new_node) {
			return new_node;
		} else {
			return m_child[1]->insert(sprite, flag / 4);
		}
	}
	else
	{
		if (m_sprite) {
			return NULL;
		}

		const float s = SCALE,
			p = PADDING * 2;
		int width = (int)(sprite->size->width * s + p + 0.5f),
			height = (int)(sprite->size->height * s + p + 0.5f);
		if (width > m_rc.width || height > m_rc.height) {
			return NULL;
		}

		m_child[0] = new Node();
		m_child[1] = new Node();

		const int dw = m_rc.width - width,
			dh = m_rc.height - height;
		m_child[0]->m_rc = m_child[1]->m_rc = m_rc;
		if ((flag % 2 == 0) && dw > dh)
		{
			m_child[0]->m_rc.width = width;
			m_child[1]->m_rc.x = m_rc.x + width;
			m_child[1]->m_rc.width -= width;
		}
		else
		{
			m_child[0]->m_rc.height = height;
			m_child[1]->m_rc.y = m_rc.y + height;
			m_child[1]->m_rc.height -= height;
		}

		// set sprite
		sprite->pos->width = width;
		sprite->pos->height = height;
		sprite->pos->x = m_child[0]->m_rc.x;
		sprite->pos->y = m_child[0]->m_rc.y;

		if (m_child[0]->m_rc.width > width)
		{
			m_child[0]->m_child[0] = new Node();
			m_child[0]->m_child[1] = new Node();

			m_child[0]->m_child[0]->m_rc = m_child[0]->m_child[1]->m_rc = m_child[0]->m_rc;
			m_child[0]->m_child[0]->m_rc.width = width;
			m_child[0]->m_child[1]->m_rc.x = m_child[0]->m_rc.x + width;
			m_child[0]->m_child[1]->m_rc.width -= width;

			m_child[0]->m_child[0]->m_sprite = sprite;

			return m_child[0]->m_child[0];
		}
		else if (m_child[0]->m_rc.height > height)
		{
			m_child[0]->m_child[0] = new Node();
			m_child[0]->m_child[1] = new Node();

			m_child[0]->m_child[0]->m_rc = m_child[0]->m_child[1]->m_rc = m_child[0]->m_rc;
			m_child[0]->m_child[0]->m_rc.height = height;
			m_child[0]->m_child[1]->m_rc.y = m_child[0]->m_rc.y + height;
			m_child[0]->m_child[1]->m_rc.height -= height;

			m_child[0]->m_child[0]->m_sprite = sprite;

			return m_child[0]->m_child[0];
		}
		else
		{
			m_child[0]->m_sprite = sprite;
			return m_child[0];
		}
	}
}

}