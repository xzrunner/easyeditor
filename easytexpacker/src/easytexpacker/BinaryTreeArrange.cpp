#include "BinaryTreeArrange.h"
#include "Context.h"

using namespace etexpacker;

BinaryTreeArrange::BinaryTreeArrange()
{
	m_root = NULL;
}

BinaryTreeArrange::~BinaryTreeArrange()
{
	delete m_root;
}

void BinaryTreeArrange::arrange(const std::vector<ee::ImageSprite*>& sprites)
{
	std::vector<ee::ImageSprite*> sorted(sprites);
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
					ee::Rect r = sorted[j]->GetSymbol().GetSize();
					ee::Vector pos;
					pos.x = -SPACING - r.Width() * 0.5f - r.CenterX();
					pos.y = -SPACING - r.Height() * 0.5f - r.CenterY();
					sorted[j]->SetTransform(pos, 0);
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
	m_root->rc.xmin = m_root->rc.ymin = 0;
	m_root->rc.xmax = Context::Instance()->width;
	m_root->rc.ymax = Context::Instance()->height;
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
BinaryTreeArrange::Node::insert(ee::ImageSprite* sprite, int flag)
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
		float width = sprite->GetSymbol().GetSize().Width() * s + p,
			height = sprite->GetSymbol().GetSize().Height() * s + p;
		if (sprite->GetAngle() != 0)
		{
			float tmp = width;
			width = height;
			height = tmp;
		}
		if (width > rc.Width() || height > rc.Height()) return NULL;

		child[0] = new Node();
		child[1] = new Node();

		const float dw = rc.Width() - width,
			dh = rc.Height() - height;
		child[0]->rc = child[1]->rc = rc;
		if ((flag % 2 == 0) && dw > dh)
		{
			child[0]->rc.xmax = rc.xmin + width;
			child[1]->rc.xmin = rc.xmin + width;
		}
		else
		{
			child[0]->rc.ymax = rc.ymin + height;
			child[1]->rc.ymin = rc.ymin + height;
		}
		ee::Vector center;
		center.x = child[0]->rc.xmin + width * 0.5f;
		center.y = child[0]->rc.ymin + height * 0.5f;
		sprite->SetTransform(center, sprite->GetAngle());

		if (child[0]->rc.Width() > width)
		{
			child[0]->child[0] = new Node();
			child[0]->child[1] = new Node();

			child[0]->child[0]->rc = child[0]->child[1]->rc = child[0]->rc;
			child[0]->child[0]->rc.xmax = child[0]->rc.xmin + width;
			child[0]->child[1]->rc.xmin = child[0]->rc.xmin + width;

			child[0]->child[0]->texture = sprite;

			return child[0]->child[0];
		}
		else if (child[0]->rc.Height() > height)
		{
			child[0]->child[0] = new Node();
			child[0]->child[1] = new Node();

			child[0]->child[0]->rc = child[0]->child[1]->rc = child[0]->rc;
			child[0]->child[0]->rc.ymax = child[0]->rc.ymin + height;
			child[0]->child[1]->rc.ymin = child[0]->rc.ymin + height;

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