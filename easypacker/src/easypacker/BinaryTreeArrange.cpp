#include "BinaryTreeArrange.h"
#include "Context.h"

using namespace epacker;

BinaryTreeArrange::BinaryTreeArrange()
{
	m_root = NULL;
}

BinaryTreeArrange::~BinaryTreeArrange()
{
	delete m_root;
}

void BinaryTreeArrange::arrange(const std::vector<d2d::ImageSprite*>& sprites)
{
	std::vector<d2d::ImageSprite*> sorted(sprites);
	sortByMaxEdge(sorted);

	int maxArranged = -1, maxFlag = 0;

	int flag = 0;
	while (flag < REINSERT_MAX)
	{
		resetRoot();

		size_t i = 0, n = sorted.size();
		for ( ; i < n; ++i)
		{
			Node* result = m_root->insert(sorted[i], flag);
			if (!result)
			{
				++flag;
				break;
			}
		}

		if (i == n)
		{
			break;
		}
		else if (i > maxArranged)
		{
			maxArranged = i;
			maxFlag = flag - 1;
		}
	}

	if (flag >= REINSERT_MAX)
	{
		resetRoot();

		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			Node* result = m_root->insert(sorted[i], maxFlag);
			if (!result)
			{
				for (size_t j = i; j < n; ++j)
				{
					d2d::Rect r = sorted[j]->getSymbol().GetSize();
					d2d::Vector pos;
					pos.x = -SPACING - r.xLength() * 0.5f - r.xCenter();
					pos.y = -SPACING - r.yLength() * 0.5f - r.yCenter();
					sorted[j]->setTransform(pos, 0);
				}
				break;
			}
		}
	}
}

void BinaryTreeArrange::resetRoot()
{
	delete m_root;
	m_root = new Node;
	m_root->rc.xMin = m_root->rc.yMin = 0;
	m_root->rc.xMax = Context::Instance()->width;
	m_root->rc.yMax = Context::Instance()->height;
}

BinaryTreeArrange::Node::Node()
{
	child[0] = child[1] = NULL;
	texture = NULL;
}

BinaryTreeArrange::Node::~Node()
{
	delete child[0];
	delete child[1];
}

BinaryTreeArrange::Node* 
BinaryTreeArrange::Node::insert(d2d::ImageSprite* sprite, int flag)
{
	if (child[0])
	{
		Node* newNode = child[0]->insert(sprite, flag / 2);
		if (newNode) return newNode;
		return child[1]->insert(sprite, flag / 4);
	}
	else
	{
		if (texture) return NULL;

		const float s = Context::Instance()->scale,
			p = Context::Instance()->padding * 2;
		float width = sprite->getSymbol().GetSize().xLength() * s + p,
			height = sprite->getSymbol().GetSize().yLength() * s + p;
		if (sprite->getAngle() != 0)
		{
			float tmp = width;
			width = height;
			height = tmp;
		}
		if (width > rc.xLength() || height > rc.yLength()) return NULL;

		child[0] = new Node();
		child[1] = new Node();

		const float dw = rc.xLength() - width,
			dh = rc.yLength() - height;
		child[0]->rc = child[1]->rc = rc;
		if ((flag % 2 == 0) && dw > dh)
		{
			child[0]->rc.xMax = rc.xMin + width;
			child[1]->rc.xMin = rc.xMin + width;
		}
		else
		{
			child[0]->rc.yMax = rc.yMin + height;
			child[1]->rc.yMin = rc.yMin + height;
		}
		d2d::Vector center;
		center.x = child[0]->rc.xMin + width * 0.5f;
		center.y = child[0]->rc.yMin + height * 0.5f;
		sprite->setTransform(center, sprite->getAngle());

		if (child[0]->rc.xLength() > width)
		{
			child[0]->child[0] = new Node();
			child[0]->child[1] = new Node();

			child[0]->child[0]->rc = child[0]->child[1]->rc = child[0]->rc;
			child[0]->child[0]->rc.xMax = child[0]->rc.xMin + width;
			child[0]->child[1]->rc.xMin = child[0]->rc.xMin + width;

			child[0]->child[0]->texture = sprite;

			return child[0]->child[0];
		}
		else if (child[0]->rc.yLength() > height)
		{
			child[0]->child[0] = new Node();
			child[0]->child[1] = new Node();

			child[0]->child[0]->rc = child[0]->child[1]->rc = child[0]->rc;
			child[0]->child[0]->rc.yMax = child[0]->rc.yMin + height;
			child[0]->child[1]->rc.yMin = child[0]->rc.yMin + height;

			child[0]->child[0]->texture = sprite;

			return child[0]->child[0];
		}
		else
		{
			child[0]->texture = sprite;
			return child[0];
		}
	}
}