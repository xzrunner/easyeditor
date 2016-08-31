#include "Symbol.h"
#include "Sprite.h"
#include "config.h"
#include "FileLoader.h"

#include <ee/EE_GTxt.h>
#include <ee/TPNode.h>
#include <ee/Config.h>
#include <ee/SpriteRenderer.h>
#include <ee/Math2D.h>
#include <ee/Visitor.h>

#include <easytext.h>

#include <sprite2/ComplexSymbol.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/RenderScissorStack.h>

#include <queue>

namespace ecomplex
{

Symbol::Symbol()
	: m_use_render_cache(false)
	, m_render_version(0)
	, m_render_cache_open(true)
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	bool scissor = m_scissor.xmin != 0 || m_scissor.ymin != 0 || m_scissor.xmax != 0 || m_scissor.ymax != 0;

	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

	if (scissor) {
		sm::vec2 min = p.mt * sm::vec2(m_scissor.xmin, m_scissor.ymin),
			     max = p.mt * sm::vec2(m_scissor.xmax, m_scissor.ymax);
		if (min.x > max.x) {
			std::swap(min.x, max.x);
		}
		if (min.y > max.y) {
			std::swap(min.y, max.y);
		}
		s2::RenderScissorStack::Instance()->Push(min.x, min.y, max.x-min.x, max.y-min.y);
	}

 	const ee::TPNode* n = NULL;
	if (ee::Config::Instance()->IsUseDTex() && 
		m_render_cache_open) 
	{
// 		ee::DynamicTexAndFont* dtex = ee::DynamicTexAndFont::Instance();
// 		n = dtex->Query(m_filepath);
	}
 	if (n) 
 	{
		//ee::DynamicTexAndFont* dtex = ee::DynamicTexAndFont::Instance();

		//ee::ShaderMgr* mgr = ee::ShaderMgr::Instance();
		//if (mgr->GetVersion() != m_render_version)
		//{
		//	m_render_cache_open = false;
		//	dtex->RefreshSymbol(*this, *n);
		//	m_render_cache_open = true;

		//	m_render_version = mgr->GetVersion();
		//}

		//sm::vec2 vertices[4];
		//float hw = m_rect.Width() * 0.5f,
		//	hh = m_rect.Height() * 0.5f;
		//vertices[0] = mt * sm::vec2(m_rect.xmin, m_rect.ymin);
		//vertices[1] = mt * sm::vec2(m_rect.xmax, m_rect.ymin);
		//vertices[2] = mt * sm::vec2(m_rect.xmax, m_rect.ymax);
		//vertices[3] = mt * sm::vec2(m_rect.xmin, m_rect.ymax);
		//if (n->IsRotated())
		//{
		//	sm::vec2 tmp = vertices[3];
		//	vertices[3] = vertices[2];
		//	vertices[2] = vertices[1];
		//	vertices[1] = vertices[0];
		//	vertices[0] = tmp;
		//}

		//sm::vec2 texcoords[4];
		//float txmin, txmax, tymin, tymax;
		//float extend = dtex->GetExtend();
		//int width = dtex->GetWidth();
		//int height = dtex->GetHeight();
		//int texid = dtex->GetTextureID();
		//txmin = (n->GetMinX()+extend) / width;
		//txmax = (n->GetMaxX()-extend) / width;
		//tymin = (n->GetMinY()+extend) / height;
		//tymax = (n->GetMaxY()-extend) / height;

		//if (texid != 1) {
		//	wxLogDebug(_T("img dt's tex = %d"), texid);
		//}
		//texcoords[0].set(txmin, tymin);
		//texcoords[1].set(txmax, tymin);
		//texcoords[2].set(txmax, tymax);
		//texcoords[3].set(txmin, tymax);

		//mgr->sprite();
		//mgr->Draw(vertices, texcoords, texid);
 	}
 	else
	{
		for (int i = 0, n = m_children.size(); i < n; ++i) {
			ee::SpriteRenderer::Instance()->Draw(m_children[i], p);
		}
	}

	if (scissor) {
		s2::RenderScissorStack::Instance()->Pop();
	}
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(m_children[i]);
		if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(child)) {
			//			// todo
			//			ee::GTxt::Instance()->Reload(text);
		}
	}
	std::set<const ee::Symbol*> syms;
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		syms.insert(dynamic_cast<const ee::Symbol*>(m_children[i]->GetSymbol()));
	}
	std::set<const ee::Symbol*>::iterator itr = syms.begin();
	for ( ; itr != syms.end(); ++itr) {
		(*itr)->ReloadTexture();
	}
}

void Symbol::Traverse(ee::Visitor<ee::Sprite>& visitor)
{
	const std::vector<s2::Sprite*>& children = GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		bool next;
		visitor.Visit(child, next);
		dynamic_cast<ee::Symbol*>(child->GetSymbol())->Traverse(visitor);
	}
}

void Symbol::LoadResources()
{
	FileLoader::Load(m_filepath, this);
}

}