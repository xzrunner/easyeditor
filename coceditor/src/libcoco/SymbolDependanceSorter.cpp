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

#include <sprite2/Sprite.h>

namespace ecoco
{

void SymbolDependanceSorter::prepare(const std::vector<ee::SymPtr>& syms)
{
	fetch(syms);
	sort();
}

void SymbolDependanceSorter::prepare(const std::vector<ee::SprPtr>& sprs)
{
	std::vector<ee::SymPtr> syms;
	syms.reserve(sprs.size());
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		syms.push_back(std::dynamic_pointer_cast<ee::Symbol>(sprs[i]->GetSymbol()));
	}
	prepare(syms);
}

void SymbolDependanceSorter::fetch(const std::vector<ee::SymPtr>& syms)
{
	for (size_t i = 0, n = syms.size(); i < n; ++i) {
		m_unique.insert(syms[i]);
	}

	std::queue<ee::SymPtr> buffer;
	for (size_t i = 0, n = syms.size(); i < n; ++i)
	{
		const ee::SymPtr& sym = syms[i];
		if (auto complex = std::dynamic_pointer_cast<const ecomplex::Symbol>(sym))
		{
			auto& children = complex->GetAllChildren();
			for (size_t j = 0, n = children.size(); j < n; ++j) {
				buffer.push(std::dynamic_pointer_cast<ee::Symbol>(children[i]->GetSymbol()));
			}
		}
		else if (auto anim = std::dynamic_pointer_cast<const libanim::Symbol>(sym))
		{
			const auto& layers = anim->GetLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				const auto& layer = layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					const auto& frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprs.size(); k < l; ++k) {
						buffer.push(std::dynamic_pointer_cast<ee::Symbol>(frame->sprs[k]->GetSymbol()));
					}
				}
			}
		}
		else if (auto patch = std::dynamic_pointer_cast<const escale9::Symbol>(sym))
		{
			CU_VEC<s2::SprPtr> grids;
			patch->GetScale9().GetGrids(grids);
			for (int i = 0, n = grids.size(); i < n; ++i) {
				buffer.push(std::dynamic_pointer_cast<ee::Symbol>(grids[i]->GetSymbol()));				
			}
		}
	}

	while (!buffer.empty())
	{
		const ee::SymPtr& sym = buffer.front(); buffer.pop();
		if (auto image = std::dynamic_pointer_cast<ee::ImageSymbol>(sym))
		{
			m_unique.insert(image);
		}
		else if (auto font = std::dynamic_pointer_cast<ee::FontBlankSymbol>(sym))
		{
			m_unique.insert(font);
		}
		else if (auto complex = std::dynamic_pointer_cast<ecomplex::Symbol>(sym))
		{
			if (m_unique.find(complex) == m_unique.end())
			{
				m_unique.insert(complex);
				auto& children = complex->GetAllChildren();
				for (size_t i = 0, n = children.size(); i < n; ++i)
				{
					const ee::SymPtr& sym = std::dynamic_pointer_cast<ee::Symbol>(children[i]->GetSymbol());
					buffer.push(sym);

					// patch for scale9
					if (auto scale9 = std::dynamic_pointer_cast<escale9::Symbol>(sym)) {
						PrepareScale9(buffer, scale9);
					}
				}
			}
		}
		else if (auto anim = std::dynamic_pointer_cast<libanim::Symbol>(sym))
		{
			if (m_unique.find(anim) == m_unique.end())
			{
				m_unique.insert(anim);
				const auto& layers = anim->GetLayers();
				for (size_t i = 0, n = layers.size(); i < n; ++i)
				{
					const auto& layer = layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
					{
						const auto& frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprs.size(); k < l; ++k)
						{
							const ee::SymPtr& sym = std::dynamic_pointer_cast<ee::Symbol>(frame->sprs[k]->GetSymbol());
							buffer.push(sym);

							// patch for scale9
							if (auto scale9 = std::dynamic_pointer_cast<escale9::Symbol>(sym)) {
								PrepareScale9(buffer, scale9);
							}
						}
					}
				}
			}
		}
		else if (auto scale9 = std::dynamic_pointer_cast<escale9::Symbol>(sym))
		{
			PrepareScale9(buffer, scale9);
		}
		else if (auto mesh = std::dynamic_pointer_cast<emesh::Symbol>(sym))
		{
			m_unique.insert(mesh);
		}
		else if (auto ocean = std::dynamic_pointer_cast<eterrain2d::Symbol>(sym))
		{
			m_unique.insert(ocean);
		}
		else if (auto tex = std::dynamic_pointer_cast<etexture::Symbol>(sym))
		{
			m_unique.insert(tex);
		}
		else if (auto icon = std::dynamic_pointer_cast<eicon::Symbol>(sym))
		{
			auto img_sym = std::dynamic_pointer_cast<const ee::ImageSymbol>(icon->GetIcon()->GetImage());
			auto sym = ee::SymbolMgr::Instance()->FetchSymbol(img_sym->GetFilepath());
			m_unique.insert(sym);
			m_unique.insert(icon);
		}
	}
}

void SymbolDependanceSorter::sort()
{
	while (!m_unique.empty())
	{
		std::set<ee::SymPtr>::iterator itr = m_unique.begin();
		for ( ; itr != m_unique.end(); ++itr)
		{
			auto& sym = *itr;
			if (auto image = std::dynamic_pointer_cast<ee::ImageSymbol>(sym))
			{
				std::string path = sym->GetFilepath();
				m_symbol_set.Insert(image);
				m_unique.erase(itr);
				break;
			}
			else if (auto font = std::dynamic_pointer_cast<ee::FontBlankSymbol>(sym))
			{
				m_symbol_set.Insert(font);
				m_unique.erase(itr);
				break;
			}
			else if (auto complex = std::dynamic_pointer_cast<ecomplex::Symbol>(sym))
			{
				bool prepared = true;
				auto& children = complex->GetAllChildren();
				for (size_t i = 0, n = children.size(); i < n && prepared; ++i) {
					auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
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
			else if (auto anim = std::dynamic_pointer_cast<libanim::Symbol>(sym))
			{
				bool prepared = true;
				const auto& layers = anim->GetLayers();
				for (size_t i = 0, n = layers.size(); i < n && prepared; ++i)
				{
					const auto& layer = layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m && prepared; ++j)
					{
						const auto& frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprs.size(); k < l && prepared; ++k) {
							auto& spr = std::dynamic_pointer_cast<ee::Sprite>(frame->sprs[k]);
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
			else if (auto patch9 = std::dynamic_pointer_cast<escale9::Symbol>(sym))
			{
 				bool prepared = true;
				CU_VEC<s2::SprPtr> grids;
				patch9->GetScale9().GetGrids(grids);
				for (int i = 0, n = grids.size(); i < n; ++i) {
					if (!IsSymbolPrepared(std::dynamic_pointer_cast<const ee::Sprite>(grids[i])))
						prepared = false;
				}
 				if (prepared)
 				{
 					m_symbol_set.Insert(patch9);
 					m_unique.erase(itr);
 					break;
 				}
			}
			else if (auto mesh = std::dynamic_pointer_cast<emesh::Symbol>(sym))
			{
				auto base = mesh->GetMesh()->GetBaseSymbol();
				std::string path = std::dynamic_pointer_cast<const ee::Symbol>(base)->GetFilepath();
 				auto image = ee::SymbolMgr::Instance()->FetchSymbol(path);
				m_symbol_set.Insert(image);
				m_symbol_set.Insert(mesh);
				m_unique.erase(itr);
				break;
			}
			else if (auto ocean_symbol = std::dynamic_pointer_cast<eterrain2d::Symbol>(sym))
			{
				const std::vector<eterrain2d::OceanMesh*> oceans = ocean_symbol->GetOceans();
				assert(oceans.size() == 1);
				eterrain2d::OceanMesh* ocean = oceans[0];
				const std::shared_ptr<ee::ImageSymbol>& img = ocean->GetImage0();
				std::string path = img->GetFilepath();
				auto image = ee::SymbolMgr::Instance()->FetchSymbol(path);
				m_symbol_set.Insert(image);
				m_symbol_set.Insert(ocean_symbol);
				m_unique.erase(itr);
				break;
			}
			else if (auto tex = std::dynamic_pointer_cast<etexture::Symbol>(sym))
			{
				auto& polys = tex->GetPolygons();
				assert(polys.size() == 1);
				for (int i = 0, n = polys.size(); i < n; ++i)
				{
					auto poly = std::dynamic_pointer_cast<eshape::PolygonShape>(polys[i]);
					assert(poly);
					const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
					assert(material);
					auto image = std::dynamic_pointer_cast<const ee::ImageSymbol>(material->GetImage());
					m_symbol_set.Insert(image);
				}
				m_symbol_set.Insert(tex);
				m_unique.erase(itr);
				break;
			}
			else if (auto icon = std::dynamic_pointer_cast<eicon::Symbol>(sym))
			{
				auto img_sym = std::dynamic_pointer_cast<const ee::ImageSymbol>(icon->GetIcon()->GetImage());
				auto sym = ee::SymbolMgr::Instance()->FetchSymbol(img_sym->GetFilepath());
				if (IsSymbolPrepared(sym)) {
 					m_symbol_set.Insert(icon);
 					m_unique.erase(itr);
 					break;
				}
			}
		}
	}
}

bool SymbolDependanceSorter::IsSymbolPrepared(const ee::SprConstPtr& spr) const
{
	return IsSymbolPrepared(std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol()));
}

bool SymbolDependanceSorter::IsSymbolPrepared(const ee::SymConstPtr& sym) const
{
	return m_symbol_set.Query(sym);
}

void SymbolDependanceSorter::PrepareScale9(std::queue<ee::SymPtr>& buffer,
										   const std::shared_ptr<escale9::Symbol>& scale9)
{
	if (m_unique.find(scale9) == m_unique.end())
	{
		m_unique.insert(scale9);		
		CU_VEC<s2::SprPtr> grids;
		scale9->GetScale9().GetGrids(grids);
		for (int i = 0, n = grids.size(); i < n; ++i) {
			buffer.push(std::dynamic_pointer_cast<ee::Symbol>(grids[i]->GetSymbol()));
		}
	}
}

}