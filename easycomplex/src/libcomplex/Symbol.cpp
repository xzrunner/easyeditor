#include "Symbol.h"
#include "Sprite.h"
#include "config.h"
#include "FileLoader.h"

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
	m_style.color.Set(0, 0.8f, 0);
}

Symbol::~Symbol()
{
	Clear();
}

void Symbol::Retain() const
{
	d2d::Object::Retain();
	for_each(m_sprites.begin(), m_sprites.end(), d2d::RetainObjectFunctor<d2d::Sprite>());
}

void Symbol::Release() const
{
	d2d::Object::Release();
	for_each(m_sprites.begin(), m_sprites.end(), d2d::ReleaseObjectFunctor<d2d::Sprite>());
}

void Symbol::ReloadTexture() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(m_sprites[i])) {
			etext::GTxt::Instance()->Reload(text);
		}
	}

	std::set<const Symbol*> symbols;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		symbols.insert(&m_sprites[i]->GetSymbol());
	}
	std::set<const Symbol*>::iterator itr = symbols.begin();
	for ( ; itr != symbols.end(); ++itr) {
		(*itr)->ReloadTexture();
	}
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
 	const d2d::TPNode* n = NULL;
	if (d2d::Config::Instance()->IsUseDTex() && 
		m_render_cache_open) 
	{
// 		d2d::DynamicTexAndFont* dtex = d2d::DynamicTexAndFont::Instance();
// 		n = dtex->Query(m_filepath);
	}
 	if (n) 
 	{
		//d2d::DynamicTexAndFont* dtex = d2d::DynamicTexAndFont::Instance();

		//d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
		//if (shader->GetVersion() != m_render_version)
		//{
		//	m_render_cache_open = false;
		//	dtex->RefreshSymbol(*this, *n);
		//	m_render_cache_open = true;

		//	m_render_version = shader->GetVersion();
		//}

		//d2d::Vector vertices[4];
		//float hw = m_rect.xLength() * 0.5f,
		//	hh = m_rect.yLength() * 0.5f;
		//vertices[0] = d2d::Math2D::transVector(d2d::Vector(m_rect.xmin, m_rect.ymin), mt);
		//vertices[1] = d2d::Math2D::transVector(d2d::Vector(m_rect.xmax, m_rect.ymin), mt);
		//vertices[2] = d2d::Math2D::transVector(d2d::Vector(m_rect.xmax, m_rect.ymax), mt);
		//vertices[3] = d2d::Math2D::transVector(d2d::Vector(m_rect.xmin, m_rect.ymax), mt);
		//if (n->IsRotated())
		//{
		//	d2d::Vector tmp = vertices[3];
		//	vertices[3] = vertices[2];
		//	vertices[2] = vertices[1];
		//	vertices[1] = vertices[0];
		//	vertices[0] = tmp;
		//}

		//d2d::Vector texcoords[4];
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

		//shader->sprite();
		//shader->Draw(vertices, texcoords, texid);
 	}
 	else
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
			d2d::SpriteRenderer::Instance()->Draw(m_sprites[i], root, mt, color);
		}
		if (m_clipbox.Width() > 0 && m_clipbox.Height() > 0) {
			d2d::PrimitiveDraw::DrawRect(mt, m_clipbox, m_style);
		}
	}
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite/* = NULL*/) const
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
		std::vector<d2d::Vector> vertices;
		m_sprites[i]->GetBounding()->GetBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.Combine(vertices[j]);
	}

	// 为兼容老数据，临时去掉
	//// to center
	//float x = m_rect.xCenter(),
	//	y = m_rect.yCenter();
	//for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	//	m_sprites[i]->translate(d2d::Vector(-x, -y));
	//m_rect.translate(d2d::Vector(-x, -y));
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