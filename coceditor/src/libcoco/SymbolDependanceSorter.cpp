#include "SymbolDependanceSorter.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>

#include <ee/ImageSymbol.h>
#include <ee/FontBlankSymbol.h>
#include <ee/Image.h>
#include <ee/SymbolMgr.h>

#include <sprite2/S2_Sprite.h>

namespace ecoco
{

void SymbolDependanceSorter::prepare(const std::vector<const ee::Symbol*>& syms)
{
	fetch(syms);
	sort();
}

void SymbolDependanceSorter::prepare(const std::vector<ee::Sprite*>& sprs)
{
	std::vector<const ee::Symbol*> syms;
	syms.reserve(sprs.size());
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		syms.push_back(dynamic_cast<const ee::Symbol*>(sprs[i]->GetSymbol()));
	}
	prepare(syms);
}

void SymbolDependanceSorter::fetch(const std::vector<const ee::Symbol*>& syms)
{
	for (size_t i = 0, n = syms.size(); i < n; ++i) {
		m_unique.insert(syms[i]);
	}

	std::queue<const ee::Symbol*> buffer;
	for (size_t i = 0, n = syms.size(); i < n; ++i)
	{
		const ee::Symbol* sym = syms[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym))
		{
			const std::vector<s2::Sprite*>& children = complex->GetChildren();
			for (size_t j = 0, n = children.size(); j < n; ++j) {
				buffer.push(dynamic_cast<const ee::Symbol*>(children[i]->GetSymbol()));
			}
		}
		else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(sym))
		{
			const std::vector<s2::AnimSymbol::Layer*>& layers = anim->GetLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				s2::AnimSymbol::Layer* layer = layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					s2::AnimSymbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprs.size(); k < l; ++k) {
						buffer.push(dynamic_cast<const ee::Symbol*>(frame->sprs[k]->GetSymbol()));
					}
				}
			}
		}
		else if (const escale9::Symbol* patch = dynamic_cast<const escale9::Symbol*>(sym))
		{
			std::vector<s2::Sprite*> grids;
			patch->GetScale9().GetGrids(grids);
			for (int i = 0, n = grids.size(); i < n; ++i) {
				buffer.push(dynamic_cast<const ee::Symbol*>(grids[i]->GetSymbol()));				
			}
		}
	}

	while (!buffer.empty())
	{
		const ee::Symbol* sym = buffer.front(); buffer.pop();
		if (const ee::ImageSymbol* image = dynamic_cast<const ee::ImageSymbol*>(sym))
		{
			m_unique.insert(image);
		}
		else if (const ee::FontBlankSymbol* font = dynamic_cast<const ee::FontBlankSymbol*>(sym))
		{
			m_unique.insert(font);
		}
		else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym))
		{
			if (m_unique.find(complex) == m_unique.end())
			{
				m_unique.insert(complex);
				const std::vector<s2::Sprite*>& children = complex->GetChildren();
				for (size_t i = 0, n = children.size(); i < n; ++i)
				{
					const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(children[i]->GetSymbol());
					buffer.push(sym);

					// patch for scale9
					if (const escale9::Symbol* scale9 = dynamic_cast<const escale9::Symbol*>(sym)) {
						PrepareScale9(buffer, scale9);
					}
				}
			}
		}
		else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(sym))
		{
			if (m_unique.find(anim) == m_unique.end())
			{
				m_unique.insert(anim);
				const std::vector<s2::AnimSymbol::Layer*>& layers = anim->GetLayers();
				for (size_t i = 0, n = layers.size(); i < n; ++i)
				{
					s2::AnimSymbol::Layer* layer = layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
					{
						s2::AnimSymbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprs.size(); k < l; ++k)
						{
							const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(frame->sprs[k]->GetSymbol());
							buffer.push(sym);

							// patch for scale9
							if (const escale9::Symbol* scale9 = dynamic_cast<const escale9::Symbol*>(sym)) {
								PrepareScale9(buffer, scale9);
							}
						}
					}
				}
			}
		}
		else if (const escale9::Symbol* scale9 = dynamic_cast<const escale9::Symbol*>(sym))
		{
			PrepareScale9(buffer, scale9);
		}
		else if (const emesh::Symbol* mesh = dynamic_cast<const emesh::Symbol*>(sym))
		{
			m_unique.insert(mesh);
		}
		else if (const eterrain2d::Symbol* ocean = dynamic_cast<const eterrain2d::Symbol*>(sym))
		{
			m_unique.insert(ocean);
		}
		else if (const etexture::Symbol* tex = dynamic_cast<const etexture::Symbol*>(sym))
		{
			m_unique.insert(tex);
		}
		else if (const eicon::Symbol* icon = dynamic_cast<const eicon::Symbol*>(sym))
		{
			const std::string& filepath = dynamic_cast<const eicon::Icon*>(icon->GetIcon())->GetImage()->GetFilepath();
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_unique.insert(sym);
			m_unique.insert(icon);
		}
	}
}

void SymbolDependanceSorter::sort()
{
	while (!m_unique.empty())
	{
		std::set<const ee::Symbol*>::iterator itr = m_unique.begin();
		for ( ; itr != m_unique.end(); ++itr)
		{
			ee::Symbol* sym = const_cast<ee::Symbol*>(*itr);
			if (ee::ImageSymbol* image = dynamic_cast<ee::ImageSymbol*>(sym))
			{
				std::string path = sym->GetFilepath();
				m_symbol_set.Insert(image);
				m_unique.erase(itr);
				break;
			}
			else if (ee::FontBlankSymbol* font = dynamic_cast<ee::FontBlankSymbol*>(sym))
			{
				m_symbol_set.Insert(font);
				m_unique.erase(itr);
				break;
			}
			else if (ecomplex::Symbol* complex = dynamic_cast<ecomplex::Symbol*>(sym))
			{
				bool prepared = true;
				const std::vector<s2::Sprite*>& children = complex->GetChildren();
				for (size_t i = 0, n = children.size(); i < n && prepared; ++i) {
					ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
					if (!IsSymbolPrepared(child)) {
						prepared = false;
					}
				}
				if (prepared)
				{
					m_symbol_set.Insert(complex);
					m_unique.erase(itr);
					break;
				}
			}
			else if (eanim::Symbol* anim = dynamic_cast<eanim::Symbol*>(sym))
			{
				bool prepared = true;
				const std::vector<s2::AnimSymbol::Layer*>& layers = anim->GetLayers();
				for (size_t i = 0, n = layers.size(); i < n && prepared; ++i)
				{
					s2::AnimSymbol::Layer* layer = layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m && prepared; ++j)
					{
						s2::AnimSymbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprs.size(); k < l && prepared; ++k) {
							ee::Sprite* spr = dynamic_cast<ee::Sprite*>(frame->sprs[k]);
							if (!IsSymbolPrepared(spr)) {
								prepared = false;
							}
						}
					}
				}
				if (prepared)
				{
					m_symbol_set.Insert(anim);
					m_unique.erase(itr);
					break;
				}
			}
			else if (escale9::Symbol* patch9 = dynamic_cast<escale9::Symbol*>(sym))
			{
 				bool prepared = true;
				std::vector<s2::Sprite*> grids;
				patch9->GetScale9().GetGrids(grids);
				for (int i = 0, n = grids.size(); i < n; ++i) {
					if (!IsSymbolPrepared(dynamic_cast<const ee::Sprite*>(grids[i])))
						prepared = false;
				}
 				if (prepared)
 				{
 					m_symbol_set.Insert(patch9);
 					m_unique.erase(itr);
 					break;
 				}
			}
			else if (emesh::Symbol* mesh = dynamic_cast<emesh::Symbol*>(sym))
			{
				const s2::Symbol* base = mesh->GetMesh()->GetBaseSymbol();
				std::string path = dynamic_cast<const ee::Symbol*>(base)->GetFilepath();
 				ee::Symbol* image = ee::SymbolMgr::Instance()->FetchSymbol(path);
				m_symbol_set.Insert(image);
				m_symbol_set.Insert(mesh);
				m_unique.erase(itr);
				break;
			}
			else if (eterrain2d::Symbol* ocean_symbol = dynamic_cast<eterrain2d::Symbol*>(sym))
			{
				const std::vector<eterrain2d::OceanMesh*> oceans = ocean_symbol->GetOceans();
				assert(oceans.size() == 1);
				eterrain2d::OceanMesh* ocean = oceans[0];
				const ee::ImageSymbol* img = ocean->GetImage0();
				std::string path = img->GetFilepath();
				ee::Symbol* image = ee::SymbolMgr::Instance()->FetchSymbol(path);
				m_symbol_set.Insert(image);
				m_symbol_set.Insert(ocean_symbol);
				m_unique.erase(itr);
				break;
			}
			else if (etexture::Symbol* tex = dynamic_cast<etexture::Symbol*>(sym))
			{
				const std::vector<s2::PolygonShape*>& polys = tex->GetPolygons();
				assert(polys.size() == 1);
				for (int i = 0, n = polys.size(); i < n; ++i)
				{
					eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[i]);
					assert(poly);
					const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
					assert(material);
					ee::ImageSymbol* image = const_cast<ee::ImageSymbol*>(material->GetImage());
					m_symbol_set.Insert(image);
				}
				m_symbol_set.Insert(tex);
				m_unique.erase(itr);
				break;
			}
			else if (eicon::Symbol* icon = dynamic_cast<eicon::Symbol*>(sym)) 
			{
				const std::string& filepath = dynamic_cast<const eicon::Icon*>(icon->GetIcon())->GetImage()->GetFilepath();
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				if (IsSymbolPrepared(sym)) {
 					m_symbol_set.Insert(icon);
 					m_unique.erase(itr);
 					break;
				}
			}
		}
	}
}

bool SymbolDependanceSorter::IsSymbolPrepared(const ee::Sprite* spr) const
{
	return IsSymbolPrepared(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
}

bool SymbolDependanceSorter::IsSymbolPrepared(const ee::Symbol* sym) const
{
	return m_symbol_set.Query(sym);
}

void SymbolDependanceSorter::PrepareScale9(std::queue<const ee::Symbol*>& buffer,
										   const escale9::Symbol* scale9)
{
	if (m_unique.find(scale9) == m_unique.end())
	{
		m_unique.insert(scale9);		
		std::vector<s2::Sprite*> grids;
		scale9->GetScale9().GetGrids(grids);
		for (int i = 0, n = grids.size(); i < n; ++i) {
			buffer.push(dynamic_cast<const ee::Symbol*>(grids[i]->GetSymbol()));
		}
	}
}

}