#include "Symbol.h"
#include "Sprite.h"
#include "FileLoader.h"

#include <ee/TPNode.h>
#include <ee/Config.h>
#include <ee/SpriteRenderer.h>
#include <ee/Math2D.h>
#include <ee/Visitor.h>
#include <ee/StringHelper.h>
#include <ee/SettingData.h>
#include <ee/SymbolFile.h>
#include <ee/FileHelper.h>
#include <ee/SymbolType.h>

#include <easytext.h>

#include <sprite2/SymType.h>
#include <sprite2/ComplexSymbol.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/RenderScissor.h>
#include <sprite2/BoundingBox.h>

#include <gum/StringHelper.h>

#include <queue>

namespace ecomplex
{

Symbol::Symbol()
	: m_use_render_cache(false)
	, m_render_version(0)
	, m_render_cache_open(true)
{
	static int id = 0;
	m_name = ee::SymbolFile::Instance()->Tag(s2::SYM_COMPLEX) + gum::StringHelper::ToString(id++);
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	sm::vec2 scissor_sz = m_scissor.Size();
	bool scissor = scissor_sz.x > 0 && scissor_sz.y > 0 && !ee::Config::Instance()->GetSettings().visible_scissor;

	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
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
		s2::RenderScissor::Instance()->Push(min.x, min.y, max.x-min.x, max.y-min.y);
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
		int action = -1;
		if (spr) {
			action = dynamic_cast<const s2::ComplexSprite*>(spr)->GetAction();
		}
		const std::vector<s2::Sprite*>& sprs = GetSprs(action);
		for (int i = 0, n = sprs.size(); i < n; ++i) {
			ee::SpriteRenderer::Instance()->Draw(sprs[i], p);
		}
	}

	if (scissor) {
		s2::RenderScissor::Instance()->Pop();
	}

	if (ee::Config::Instance()->GetSettings().visible_scissor && scissor_sz.x > 0 && scissor_sz.y > 0) {
		sm::vec2 min(m_scissor.xmin, m_scissor.ymin), 
				 max(m_scissor.xmax, m_scissor.ymax);
		s2::RVG::SetColor(s2::Color(0, 204, 0));
		s2::RVG::Rect(p.mt * min, p.mt * max, false);
	}
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	sm::vec2 scissor_sz = m_scissor.Size();
	if (scissor_sz.x > 0 && scissor_sz.y > 0 && !ee::Config::Instance()->GetSettings().visible_scissor) {
		return m_scissor;
	}

	sm::rect b;
	int action = -1;
	if (spr) {
		action = dynamic_cast<const s2::ComplexSprite*>(spr)->GetAction();
	}
	const std::vector<s2::Sprite*>& sprs = GetSprs(action);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->GetBounding()->CombineTo(b);
	}
	return b;
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

bool Symbol::HasActions() const
{
	return !m_actions.empty();
}

void Symbol::GetActionNames(std::vector<std::string>& actions) const
{
	actions.reserve(m_actions.size());
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		actions.push_back(m_actions[i].name);
	}
}

void Symbol::LoadResources()
{
	std::string filename = ee::FileHelper::GetFilename(m_filepath);
	if (filename == ee::SYM_GROUP_TAG) {
		return;
	}

	FileLoader::Load(m_filepath, this);

	m_origin_names.clear();
	const std::vector<s2::Sprite*>& children = GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		const std::string& name = children[i]->GetName();
		if (!name.empty() && name[0] != '_') {
			m_origin_names.push_back(children[i]->GetName());
		}
	}
}

}