#include "Page.h"
#include "ImportStream.h"
#include "simp_types.h"
#include "Allocator.h"
#include "simp_define.h"

#include "NodePicture.h"
#include "NodeScale9.h"
#include "NodeScale9Spr.h"
#include "NodeIcon.h"
#include "NodeIconSpr.h"
#include "NodeTexture.h"
#include "NodeLabel.h"
#include "NodeComplex.h"

#include <new>

namespace simp
{

Page::Page(Allocator& alloc, ImportStream& is, int begin_id, int end_id)
	: m_begin_id(begin_id)
	, m_end_id(end_id)
{
	Init(alloc, is);
}

Page::~Page()
{
}

int Page::Size()
{
	return sizeof(Page) + PTR_SIZE_DIFF * 2;
}

void Page::Init(Allocator& alloc, ImportStream& is)
{
	int n = m_end_id - m_begin_id + 1;
	int align_n = (n + 3) & ~3;
	m_types = alloc.Alloc(sizeof(uint8_t) * align_n);
	memset(m_types, 0, sizeof(uint8_t) * align_n);
	m_nodes = alloc.Alloc(SIZEOF_POINTER * n);
	memset(m_nodes, 0, SIZEOF_POINTER * n);

	while (!is.Empty())
	{
		uint32_t id = is.UInt32();
		int idx = id - m_begin_id;
		m_types[idx] = is.UInt8();
		m_nodes[idx] = CreateNode(type, alloc, is);
	}
}

void* Page::CreateNode(uint16_t type, Allocator& alloc, ImportStream& is)
{
	void* ret = NULL;
	switch (type)
	{
	case TYPE_IMAGE:
		{
			void* ptr = alloc.Alloc(NodePicture::Size());
			ret = new (ptr) NodePicture(is);
		}
		break;
	case TYPE_SCALE9:
		{
			void* ptr = alloc.Alloc(NodeScale9::Size());
			ret = new (ptr) NodeScale9(alloc, is);
		}
		break;
	case TYPE_SCALE9_SPR:
		{
			void* ptr = alloc.Alloc(NodeScale9Spr::Size());
			ret = new (ptr) NodeScale9Spr(is);
		}
		break;
	case TYPE_ICON:
		{
			void* ptr = alloc.Alloc(NodeIcon::Size());
			ret = new (ptr) NodeIcon(is);
		}
		break;
	case TYPE_ICON_SPR:
		{
			void* ptr = alloc.Alloc(NodeIconSpr::Size());
			ret = new (ptr) NodeIconSpr(is);
		}
		break;
	case TYPE_TEXTURE:
		{
			void* ptr = alloc.Alloc(NodeTexture::Size());
			ret = new (ptr) NodeTexture(is);
		}
		break;
	case TYPE_COMPLEX:
		{
			void* ptr = alloc.Alloc(NodeComplex::Size());
			ret = new (ptr) NodeComplex(is);
		}
		break;
	}
	return ret;
}

}