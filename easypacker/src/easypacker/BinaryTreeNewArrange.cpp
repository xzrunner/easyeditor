#include "BinaryTreeNewArrange.h"
#include "Context.h"

namespace epacker
{

static const float PADDING = 1;

BinaryTreeNewArrange::BinaryTreeNewArrange()
	: m_root(NULL)
{
}

BinaryTreeNewArrange::~BinaryTreeNewArrange()
{
	delete m_root;
	m_mapImages.clear();
}

void BinaryTreeNewArrange::arrange(const std::vector<d2d::ImageSprite*>& sprites)
{
	float tot_w = Context::Instance()->width,
		tot_h = Context::Instance()->height;
	float scale = 1;

	std::vector<d2d::ImageSprite*> sorted(sprites);
	sortByMaxEdge(sorted);

	float x_offset = 0;
	while (!sorted.empty())
	{
		std::vector<d2d::ImageSprite*> remain;

		InitRoot();
		for (int i = 0, n = sorted.size(); i < n; ++i)
		{
			d2d::ImageSprite* s = sorted[i];
			bool success = Insert(*s);
			if (!success) {
				d2d::Rect r = s->getSymbol().getSize();
				float w = r.xLength() * scale + PADDING*2;
				float h = r.yLength() * scale + PADDING*2;
				if ((w > tot_w || h > tot_h) &&
					(w > tot_h || h > tot_w)) {
					s->setTransform(d2d::Vector(-1024, -1024), 0);
				} else {
					remain.push_back(s);
				}
			} else {
				s->translate(d2d::Vector(x_offset, 0.0f));
			}
		}
		x_offset += Context::Instance()->width * 1.5f;

		sorted = remain;
	}
}

void BinaryTreeNewArrange::InitRoot()
{
	int w = Context::Instance()->width;
	int h = Context::Instance()->height;

	delete m_root;
	m_root = new Node(w, h);

	Node* c = new Node(w, h);
	m_root->child = c;
}

bool BinaryTreeNewArrange::
Insert(d2d::ImageSprite& img) const
{
	std::map<std::string, Node*>::const_iterator itr 
		= m_mapImages.find(img.getSymbol().getFilepath().ToStdString());
	if (itr != m_mapImages.end()) {
		return false;
	}

	Node* n = NULL;
	d2d::Rect r = img.getSymbol().getImage()->getRegion();
	int w = r.xLength();
	int h = r.yLength();
	float scale = 1.0f;
	if (m_root->IsRoomEnough(r.xLength(), r.yLength())) {
		n = m_root->Insert(img, w*scale+PADDING*2, h*scale+PADDING*2);		
	} else {
		n = m_root->Insert(img, h*scale+PADDING*2, w*scale+PADDING*2);
	}

	if (!n) {
		return false;
	} else {
		d2d::Vector pos;
		pos.x = (n->xmin + n->xmax) * 0.5f - r.xCenter();
		pos.y = (n->ymin + n->ymax) * 0.5f - r.yCenter();
		float angle = n->is_rotated ? d2d::PI*0.5f : 0;
		img.setTransform(pos, angle);
		return true;
	}

// 	if (w < h) {
// 		n->is_rotated = !n->is_rotated;
// 	}
}

//////////////////////////////////////////////////////////////////////////
// struct BinaryTreeNewArrange::Node
//////////////////////////////////////////////////////////////////////////

BinaryTreeNewArrange::Node::
Node()
	: image(NULL)
{
	xmin = ymin = xmax = ymax = 0;
	is_rotated = is_split_y = false;

	next = child = parent = NULL;

	remain_area = remain_len = remain_space = 0;
}

BinaryTreeNewArrange::Node::
Node(int width, int height)
	: image(NULL)
{
	xmin = ymin = 0;
	xmax = width;
	ymax = height;
	is_rotated = is_split_y = false;

	next = child = parent = NULL;

	remain_area = width * height;
	remain_len = std::max(width, height);
	remain_space = width;
}

BinaryTreeNewArrange::Node* BinaryTreeNewArrange::Node::
Insert(const d2d::ImageSprite& img, int w, int h)
{
// 	d2d::Rect r = img.getSymbol().getImage()->getRegion();
// 	int w = r.xLength(),
// 		h = r.yLength();

	int dw = xmax - xmin,
		dh = ymax - ymin;
	if ((w > dw && h > dh) || (w > dh && h > dw)) {
		return NULL;
	}

	if (!child) 
	{
		if (image) {
			return NULL;
		}

		if (w <= dw && h <= dh) 
		{	  
			Node* n = Split(w, h);
			if (n) {
				n->image = &img;
			}
			return n;
		} 
		else if (w <= dh && h <= dw) 
		{
			Node* n = Split(h, w);
			if (n) {
				n->image = &img;
				n->is_rotated = true;
			}
			return n;
		} 
		else 
		{
			return NULL;
		}
	}
	else
	{
		Node* next = child;
		while (next) 
		{
			Node* n = NULL;
			if (next->IsRoomEnough(w, h)) {
				n = next->Insert(img, w, h);
			}
			if (n) {
				return n;
			} else {	  	
				next = next->next;
			}
		}    
		return NULL; 
	}
}

bool BinaryTreeNewArrange::Node::
IsRoomEnough(int w, int h) const
{
	return (w <= remain_space || h <= remain_space) 
		&& w*h <= remain_area 
		&& w <= remain_len && h <= remain_len;
}

BinaryTreeNewArrange::Node* BinaryTreeNewArrange::Node::
Split(int w, int h)
{
	Node* _next = new Node;
	Node* _child = new Node;
	Node* _child_next = new Node;
	assert(_child_next);

	if (is_split_y) 
	{
		_next->xmin = xmin;
		_next->xmax = xmax;
		_next->ymin = ymin+h;
		_next->ymax = ymax;
		ymax = _next->ymin;
		_next->is_split_y = true;

		_child->ymin = ymin;
		_child->ymax = ymax;
		_child->xmin = xmin;
		_child->xmax = xmin+w;
		_child->is_split_y = false;

		_child_next->ymin = ymin;
		_child_next->ymax = ymax;
		_child_next->xmin = _child->xmax;
		_child_next->xmax = xmax;
		_child_next->is_split_y = false;
	} 
	else 
	{
		_next->ymin = ymin;
		_next->ymax = ymax;
		_next->xmin = xmin+w;
		_next->xmax = xmax;
		xmax = _next->xmin;
		_next->is_split_y = false;

		_child->xmin = xmin;
		_child->xmax = xmax;
		_child->ymin = ymin;
		_child->ymax = ymin+h;
		_child->is_split_y = true;

		_child_next->xmin = xmin;
		_child_next->xmax = xmax;
		_child_next->ymin = _child->ymax;
		_child_next->ymax = ymax;
		_child_next->is_split_y = true;
	}
	next = _next;
	_next->parent = parent;
	child = _child;
	_child->parent = parent;
	_child->next = _child_next;
	_child_next->parent = this;

	// remain area
	_next->remain_area = _next->GetArea();
	_child_next->remain_area = _child_next->GetArea();
	_child->remain_area = 0;
	remain_area = _child_next->remain_area;
	// remain len
	_next->remain_len = _next->GetMaxLength();
	_child_next->remain_len = _child_next->GetMaxLength();
	_child->remain_len = 0;
	remain_len = _child_next->remain_len;  
	// remain_space
	if (is_split_y) 
	{
		_next->remain_space = _next->ymax - _next->ymin;
		_child_next->remain_space = _child_next->xmax - _child_next->xmin;
		_child->remain_space = 0;
		remain_space = _child_next->remain_space;
	} 
	else 
	{
		_next->remain_space = _next->xmax - _next->xmin;
		_child_next->remain_space = _child_next->ymax - _child_next->ymin;
		_child->remain_space = 0;
		remain_space = _child_next->remain_space;
	}
	UpdateRemain();

	return _child;
}

float BinaryTreeNewArrange::Node::
GetMaxLength() const
{
	int w = xmax - xmin,
		h = ymax - xmin;
	return w > h ? w : h;
}

float BinaryTreeNewArrange::Node::
GetArea() const
{
	return (xmax-xmin)*(ymax-ymin);
}

void BinaryTreeNewArrange::Node::
UpdateRemain()
{
	struct Node* p = parent;
	while (p) 
	{
		p->remain_area = 0;
		p->remain_len = 0;
		p->remain_space = 0;
		struct Node* c = p->child;
		while (c) 
		{
			if (c->remain_area > p->remain_area)
				p->remain_area = c->remain_area;
			if (c->remain_len > p->remain_len)
				p->remain_len = c->remain_len;
			if (c->remain_space > p->remain_space)
				p->remain_space = c->remain_space;  	  
			c = c->next;
		}
		p = p->parent;
	}
}

}