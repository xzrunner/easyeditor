#include "Symbol.h"
#include "Sprite.h"
#include "config.h"
#include "FileLoader.h"

#include <ee/EE_GTxt.h>
#include <ee/EE_RVG.h>
#include <ee/TPNode.h>
#include <ee/Config.h>
#include <ee/SpriteRenderer.h>
#include <ee/BoundingBox.h>
#include <ee/Math2D.h>

#include <easytext.h>

#include <queue>

namespace ecomplex
{

const float Symbol::SCALE = 0.15f;

Symbol::Symbol()
	: m_use_render_cache(false)
	, m_render_version(0)
	, m_render_cache_open(true)
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);

	m_clipbox.xmin = m_clipbox.xmax = m_clipbox.ymin = m_clipbox.ymax = 0;
}

Symbol::~Symbol()
{
	Clear();
}

void Symbol::Retain() const
{
	ee::Object::Retain();
	for_each(m_sprites.begin(), m_sprites.end(), ee::RetainObjectFunctor<ee::Sprite>());
}

void Symbol::Release() const
{
	ee::Object::Release();
	for_each(m_sprites.begin(), m_sprites.end(), ee::ReleaseObjectFunctor<ee::Sprite>());
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
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
		//vertices[0] = ee::Math2D::TransVector(sm::vec2(m_rect.xmin, m_rect.ymin), mt);
		//vertices[1] = ee::Math2D::TransVector(sm::vec2(m_rect.xmax, m_rect.ymin), mt);
		//vertices[2] = ee::Math2D::TransVector(sm::vec2(m_rect.xmax, m_rect.ymax), mt);
		//vertices[3] = ee::Math2D::TransVector(sm::vec2(m_rect.xmin, m_rect.ymax), mt);
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
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
			ee::SpriteRenderer::Draw(m_sprites[i], params);
		}
		if (m_clipbox.Width() > 0 && m_clipbox.Height() > 0) {
			sm::vec2 min(m_clipbox.xmin, m_clipbox.ymin), 
				max(m_clipbox.xmax, m_clipbox.ymax);
			ee::RVG::Color(s2::Color(0, 204, 0));
			ee::RVG::Rect(ee::Math2D::TransVector(min, params.mt), ee::Math2D::TransVector(max, params.mt), false);
		}
	}
}

void Symbol::ReloadTexture() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(m_sprites[i])) {
			//			// todo
			//			ee::GTxt::Instance()->Reload(text);
		}
	}

	std::set<const ee::Symbol*> symbols;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		symbols.insert(&m_sprites[i]->GetSymbol());
	}
	std::set<const ee::Symbol*>::iterator itr = symbols.begin();
	for ( ; itr != symbols.end(); ++itr) {
		(*itr)->ReloadTexture();
	}
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite/* = NULL*/) const
{
	return m_rect;
}

bool Symbol::isOneLayer() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (dynamic_cast<Sprite*>(m_sprites[i]))
			return false;
	return true;
}

void Symbol::InitBounding()
{
	m_rect.MakeInfinite();
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<sm::vec2> vertices;
		m_sprites[i]->GetBounding()->GetBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.Combine(vertices[j]);
	}

	// 为兼容老数据，临时去掉
	//// to center
	//float x = m_rect.CenterX(),
	//	y = m_rect.CenterY();
	//for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	//	m_sprites[i]->translate(sm::vec2(-x, -y));
	//m_rect.translate(sm::vec2(-x, -y));
}

void Symbol::LoadResources()
{
	FileLoader::Load(m_filepath, this);
}

void Symbol::Clear()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->Release();
	m_sprites.clear();
}

}