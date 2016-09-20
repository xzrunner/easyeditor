#include "Page.h"
#include "ImportStream.h"
#include "simp_types.h"
#include "Allocator.h"
#include "simp_define.h"
#include "StaticAlloc.h"

#include "NodePicture.h"
#include "NodeScale9.h"
#include "NodeScale9Spr.h"
#include "NodeIcon.h"
#include "NodeIconSpr.h"
#include "NodeTexture.h"
#include "NodeLabel.h"
#include "NodeComplex.h"
#include "NodeComplexSpr.h"
#include "NodeAnimation.h"
#include "NodeAnimationSpr.h"
#include "NodeParticle3d.h"
#include "NodeParticle3dSpr.h"
#include "NodeParticle2d.h"
#include "NodeParticle2dSpr.h"
#include "NodeMesh.h"
#include "NodeMeshSpr.h"
#include "NodeMask.h"
#include "NodeMaskSpr.h"
#include "NodeTrail.h"
#include "NodeTrailSpr.h"

#include <new>
#include <string.h>

#include <assert.h>

namespace simp
{

Page::Page(Allocator* alloc, int begin_id, int end_id)
	: m_alloc(alloc)
	, m_begin_id(begin_id)
	, m_end_id(end_id)
{
}

Page::~Page()
{
	StaticAlloc::Instance()->Release(m_alloc);
}

void Page::Load(const std::string& filepath)
{
	Loader loader(filepath, this);
	loader.Load();
}

const void* Page::Query(uint32_t id, int* type) const
{
	assert(id >= m_begin_id && id <= m_end_id);
	int idx = id - m_begin_id;
	*type = m_types[idx];
	return m_nodes[idx];
}

int Page::Size()
{
	return sizeof(Page) + PTR_SIZE_DIFF * 2;
}

/************************************************************************/
/* class Page::Loader                                                   */
/************************************************************************/

Page::Loader::
Loader(const std::string& filepath, Page* page)
	: FileLoader(filepath)
	, m_page(page)
{
}

void Page::Loader::
OnLoad(ImportStream& is)
{
	Allocator& alloc = *(m_page->m_alloc);

	int n = m_page->m_end_id - m_page->m_begin_id + 1;
	int align_n = ALIGN_4BYTE(n);
	m_page->m_types = static_cast<uint8_t*>(alloc.Alloc(sizeof(uint8_t) * align_n));
	memset(m_page->m_types, 0, sizeof(uint8_t) * align_n);
	m_page->m_nodes =  static_cast<void**>(alloc.Alloc(SIZEOF_POINTER * n));
	memset(m_page->m_nodes, 0, SIZEOF_POINTER * n);

	while (!is.Empty())
	{
		uint32_t id = is.UInt32();
		int idx = id - m_page->m_begin_id;
		uint8_t type = is.UInt8();
		m_page->m_types[idx] = type;
		m_page->m_nodes[idx] = CreateNode(type, alloc, is);
	}
}

void* Page::Loader::
CreateNode(uint8_t type, Allocator& alloc, ImportStream& is)
{
	void* ret = NULL;
	switch (type)
	{
	case TYPE_IMAGE:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodePicture::Size()));
			ret = new (ptr) NodePicture(is);
		}
		break;
	case TYPE_SCALE9:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeScale9::Size()));
			ret = new (ptr) NodeScale9(alloc, is);
		}
		break;
	case TYPE_SCALE9_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeScale9Spr::Size()));
			ret = new (ptr) NodeScale9Spr(is);
		}
		break;
	case TYPE_ICON:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeIcon::Size()));
			ret = new (ptr) NodeIcon(is);
		}
		break;
	case TYPE_ICON_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeIconSpr::Size()));
			ret = new (ptr) NodeIconSpr(is);
		}
		break;
	case TYPE_TEXTURE:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeTexture::Size()));
			ret = new (ptr) NodeTexture(is);
		}
		break;
	case TYPE_LABEL:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeLabel::Size()));
			ret = new (ptr) NodeLabel(alloc, is);
		}
		break;
	case TYPE_COMPLEX:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeComplex::Size()));
			ret = new (ptr) NodeComplex(alloc, is);
		}
		break;
	case TYPE_COMPLEX_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeComplexSpr::Size()));
			ret = new (ptr) NodeComplexSpr(is);
		}
		break;
	case TYPE_ANIMATION:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeAnimation::Size()));
			ret = new (ptr) NodeAnimation(alloc, is);
		}
		break;
	case TYPE_ANIM_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeAnimationSpr::Size()));
			ret = new (ptr) NodeAnimationSpr(is);
		}
		break;
	case TYPE_PARTICLE3D:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeParticle3d::Size()));
			ret = new (ptr) NodeParticle3d(alloc, is);
		}
		break;
	case TYPE_P3D_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeParticle3dSpr::Size()));
			ret = new (ptr) NodeParticle3dSpr(is);
		}
		break;
	case TYPE_PARTICLE2D:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeParticle2d::Size()));
			ret = new (ptr) NodeParticle2d(alloc, is);
		}
		break;
	case TYPE_P2D_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeParticle2dSpr::Size()));
			ret = new (ptr) NodeParticle2dSpr(is);
		}
		break;
	case TYPE_MESH:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeMesh::Size()));
			ret = new (ptr) NodeMesh(alloc, is);
		}
		break;
	case TYPE_MESH_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeMeshSpr::Size()));
			ret = new (ptr) NodeMeshSpr(alloc, is);
		}
		break;
	case TYPE_MASK:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeMask::Size()));
			ret = new (ptr) NodeMask(is);
		}
		break;
	case TYPE_MASK_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeMaskSpr::Size()));
			ret = new (ptr) NodeMaskSpr(is);
		}
		break;
	case TYPE_TRAIL:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeTrail::Size()));
			ret = new (ptr) NodeTrail(alloc, is);
		}
		break;
	case TYPE_TRAIL_SPR:
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(NodeTrailSpr::Size()));
			ret = new (ptr) NodeTrailSpr(is);
		}
		break;
	default:
		assert(0);
	}
	return ret;
}

}