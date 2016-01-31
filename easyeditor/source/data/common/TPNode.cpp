#include "TPNode.h"

#include <algorithm>

#include <stdio.h>
#include <assert.h>

namespace ee
{

TPNode::TPNode()
	: m_used(false)
{
	m_xmin = m_ymin = m_xmax = m_ymax = 0;
	m_is_rotated = m_is_split_y = false;

	m_next = m_child = m_parent = NULL;

	m_remain_area = m_remain_len = m_remain_space = 0;
}

TPNode::TPNode(int width, int height)
	: m_used(false)
{
	m_xmin = m_ymin = 0;
	m_xmax = width;
	m_ymax = height;
	m_is_rotated = m_is_split_y = false;

	m_next = m_child = m_parent = NULL;

	m_remain_area = width * height;
	m_remain_len = std::max(width, height);
	m_remain_space = width;
}

TPNode* TPNode::Insert(int w, int h)
{
// 	Rect r = img.getSymbol().getImage()->getRegion();
// 	int w = r.Width(),
// 		h = r.Height();

	int dw = m_xmax - m_xmin,
		dh = m_ymax - m_ymin;
	if ((w > dw && h > dh) || (w > dh && h > dw)) {
		return NULL;
	}

	if (!m_child) 
	{
		if (m_used) {
			return NULL;
		}

		if (w <= dw && h <= dh) 
		{	  
			TPNode* n = Split(w, h);
			if (n) {
				n->m_used = true;
			}
			return n;
		} 
		else if (w <= dh && h <= dw) 
		{
			TPNode* n = Split(h, w);
			if (n) {
				n->m_used = true;
				n->m_is_rotated = true;
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
		TPNode* next = m_child;
		while (next) 
		{
			TPNode* n = NULL;
			if (next->IsRoomEnough(w, h)) {
				n = next->Insert(w, h);
			}
			if (n) {
				return n;
			} else {	  	
				next = next->m_next;
			}
		}    
		return NULL; 
	}
}

void TPNode::Clear()
{
	m_used = false;
}

bool TPNode::IsRoomEnough(int w, int h) const
{
	return (w <= m_remain_space || h <= m_remain_space) 
		&& w*h <= m_remain_area 
		&& w <= m_remain_len && h <= m_remain_len;
}

TPNode* TPNode::Split(int w, int h)
{
	TPNode* _next = new TPNode;
	TPNode* _child = new TPNode;
	TPNode* _child_next = new TPNode;
	assert(_child_next);

	if (m_is_split_y) 
	{
		_next->m_xmin = m_xmin;
		_next->m_xmax = m_xmax;
		_next->m_ymin = m_ymin+h;
		_next->m_ymax = m_ymax;
		m_ymax = _next->m_ymin;
		_next->m_is_split_y = true;

		_child->m_ymin = m_ymin;
		_child->m_ymax = m_ymax;
		_child->m_xmin = m_xmin;
		_child->m_xmax = m_xmin+w;
		_child->m_is_split_y = false;

		_child_next->m_ymin = m_ymin;
		_child_next->m_ymax = m_ymax;
		_child_next->m_xmin = _child->m_xmax;
		_child_next->m_xmax = m_xmax;
		_child_next->m_is_split_y = false;
	} 
	else 
	{
		_next->m_ymin = m_ymin;
		_next->m_ymax = m_ymax;
		_next->m_xmin = m_xmin+w;
		_next->m_xmax = m_xmax;
		m_xmax = _next->m_xmin;
		_next->m_is_split_y = false;

		_child->m_xmin = m_xmin;
		_child->m_xmax = m_xmax;
		_child->m_ymin = m_ymin;
		_child->m_ymax = m_ymin+h;
		_child->m_is_split_y = true;

		_child_next->m_xmin = m_xmin;
		_child_next->m_xmax = m_xmax;
		_child_next->m_ymin = _child->m_ymax;
		_child_next->m_ymax = m_ymax;
		_child_next->m_is_split_y = true;
	}
	m_next = _next;
	_next->m_parent = m_parent;
	m_child = _child;
	_child->m_parent = m_parent;
	_child->m_next = _child_next;
	_child_next->m_parent = this;

	// remain area
	_next->m_remain_area = static_cast<int>(_next->GetArea());
	_child_next->m_remain_area = static_cast<int>(_child_next->GetArea());
	_child->m_remain_area = static_cast<int>(_child->GetArea());
	m_remain_area = _child_next->m_remain_area;
	// remain len
	_next->m_remain_len = static_cast<int>(_next->GetMaxLength());
	_child_next->m_remain_len = static_cast<int>(_child_next->GetMaxLength());
	_child->m_remain_len = static_cast<int>(_child->GetMaxLength());
	m_remain_len = _child_next->m_remain_len;  
	// remain_space
	if (m_is_split_y) 
	{
		_next->m_remain_space = _next->m_ymax - _next->m_ymin;
		_child_next->m_remain_space = _child_next->m_xmax - _child_next->m_xmin;
		_child->m_remain_space = _child->m_xmax - _child->m_xmin;
		m_remain_space = _child_next->m_remain_space;
	} 
	else 
	{
		_next->m_remain_space = _next->m_xmax - _next->m_xmin;
		_child_next->m_remain_space = _child_next->m_ymax - _child_next->m_ymin;
		_child->m_remain_space = _child->m_ymax - _child->m_ymin;
		m_remain_space = _child_next->m_remain_space;
	}
	UpdateRemain();

	return _child;
}

float TPNode::GetMaxLength() const
{
	float w = static_cast<float>(m_xmax - m_xmin),
		  h = static_cast<float>(m_ymax - m_ymin);
	return w > h ? w : h;
}

float TPNode::GetArea() const
{
	return static_cast<float>((m_xmax-m_xmin)*(m_ymax-m_ymin));
}

void TPNode::UpdateRemain()
{
	TPNode* p = m_parent;
	while (p) 
	{
		p->m_remain_area = 0;
		p->m_remain_len = 0;
		p->m_remain_space = 0;
		TPNode* c = p->m_child;
		while (c) 
		{
			if (c->m_remain_area > p->m_remain_area)
				p->m_remain_area = c->m_remain_area;
			if (c->m_remain_len > p->m_remain_len)
				p->m_remain_len = c->m_remain_len;
			if (c->m_remain_space > p->m_remain_space)
				p->m_remain_space = c->m_remain_space;  	  
			c = c->m_next;
		}
		p = p->m_parent;
	}
}


}