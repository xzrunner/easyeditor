#include "SymbolDependanceSorter.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>

namespace libcoco
{

void SymbolDependanceSorter::prepare(const std::vector<const d2d::ISymbol*>& symbols)
{
	fetch(symbols);
	sort();
}

void SymbolDependanceSorter::prepare(const std::vector<const d2d::ISprite*>& sprites)
{
	std::vector<const d2d::ISymbol*> symbols;
	symbols.reserve(sprites.size());
	for (int i = 0, n = sprites.size(); i < n; ++i)
		symbols.push_back(&sprites[i]->GetSymbol());
	prepare(symbols);
}

void SymbolDependanceSorter::fetch(const std::vector<const d2d::ISymbol*>& symbols)
{
	for (size_t i = 0, n = symbols.size(); i < n; ++i) {
		m_unique.insert(symbols[i]);
	}

	std::queue<const d2d::ISymbol*> buffer;
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		const d2d::ISymbol* symbol = symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol))
		{
			for (size_t j = 0, n = complex->m_sprites.size(); j < n; ++j)
				buffer.push(&complex->m_sprites[j]->GetSymbol());
		}
		else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(symbol))
		{
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				libanim::Symbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					libanim::Symbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						buffer.push(&frame->sprites[k]->GetSymbol());
				}
			}
		}
		else if (const escale9::Symbol* patch = dynamic_cast<const escale9::Symbol*>(symbol))
		{
			const escale9::Scale9Data& data = patch->GetScale9Data();
 			switch (data.GetType())
 			{
 			case escale9::e_9Grid:
 				for (size_t i = 0; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j)
 						buffer.push(&data.GetSprite(i, j)->GetSymbol());
 				break;
 			case escale9::e_9GridHollow:
 				for (size_t i = 0; i < 3; ++i) {
 					for (size_t j = 0; j < 3; ++j) {
 						if (i == 1 && j == 1) continue;
 						buffer.push(&data.GetSprite(i, j)->GetSymbol());
 					}
 				}
 				break;
 			case escale9::e_3GridHor:
 				for (size_t i = 0; i < 3; ++i)
 					buffer.push(&data.GetSprite(1, i)->GetSymbol());
 				break;
 			case escale9::e_3GridVer:
 				for (size_t i = 0; i < 3; ++i)
 					buffer.push(&data.GetSprite(i, 1)->GetSymbol());
 				break;
 			case escale9::e_6GridUpper:
 				for (size_t i = 1; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j)
 						buffer.push(&data.GetSprite(i, j)->GetSymbol());
 				break;
 			}
		}
	}

	while (!buffer.empty())
	{
		const d2d::ISymbol* symbol = buffer.front(); buffer.pop();
		if (const d2d::ImageSymbol* image = dynamic_cast<const d2d::ImageSymbol*>(symbol))
		{
			m_unique.insert(image);
		}
		else if (const d2d::FontBlankSymbol* font = dynamic_cast<const d2d::FontBlankSymbol*>(symbol))
		{
			m_unique.insert(font);
		}
		else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol))
		{
			if (m_unique.find(complex) == m_unique.end())
			{
				m_unique.insert(complex);
				for (size_t i = 0, n = complex->m_sprites.size(); i < n; ++i)
				{
					d2d::ISprite* child = complex->m_sprites[i];
					buffer.push(&child->GetSymbol());

					// patch for scale9
					if (const escale9::Symbol* scale9 = dynamic_cast<const escale9::Symbol*>(child)) {
						PrepareScale9(buffer, scale9);
					}
				}
			}
		}
		else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(symbol))
		{
			if (m_unique.find(anim) == m_unique.end())
			{
				m_unique.insert(anim);
				for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
				{
					libanim::Symbol::Layer* layer = anim->m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
					{
						libanim::Symbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						{
							d2d::ISprite* child = frame->sprites[k];
							buffer.push(&child->GetSymbol());

							// patch for scale9
							if (const escale9::Symbol* scale9 = dynamic_cast<const escale9::Symbol*>(&child->GetSymbol())) {
								PrepareScale9(buffer, scale9);
							}
						}
					}
				}
			}
		}
		else if (const escale9::Symbol* scale9 = dynamic_cast<const escale9::Symbol*>(symbol))
		{
			PrepareScale9(buffer, scale9);
		}
		else if (const emesh::Symbol* mesh = dynamic_cast<const emesh::Symbol*>(symbol))
		{
			m_unique.insert(mesh);
		}
		else if (const eterrain2d::Symbol* ocean = dynamic_cast<const eterrain2d::Symbol*>(symbol))
		{
			m_unique.insert(ocean);
		}
		else if (const etexture::Symbol* tex = dynamic_cast<const etexture::Symbol*>(symbol))
		{
			m_unique.insert(tex);
		}
		else if (const eicon::Symbol* icon = dynamic_cast<const eicon::Symbol*>(symbol))
		{
			const std::string& filepath = icon->GetIcon()->GetImage()->GetFilepath();
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_unique.insert(symbol);
			m_unique.insert(icon);
		}
	}
}

void SymbolDependanceSorter::sort()
{
	while (!m_unique.empty())
	{
		std::set<const d2d::ISymbol*>::iterator itr = m_unique.begin();
		for ( ; itr != m_unique.end(); ++itr)
		{
			d2d::ISymbol* symbol = const_cast<d2d::ISymbol*>(*itr);
			if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
			{
				std::string path = symbol->GetFilepath();
				m_result.push_back(image);
				m_unique.erase(itr);
				break;
			}
			else if (d2d::FontBlankSymbol* font = dynamic_cast<d2d::FontBlankSymbol*>(symbol))
			{
				m_result.push_back(font);
				m_unique.erase(itr);
				break;
			}
			else if (ecomplex::Symbol* complex = dynamic_cast<ecomplex::Symbol*>(symbol))
			{
				bool prepared = true;
				for (size_t i = 0, n = complex->m_sprites.size(); i < n && prepared; ++i) {
					if (!IsSymbolPrepared(complex->m_sprites[i])) {
						prepared = false;
					}
				}
				if (prepared)
				{
					m_result.push_back(complex);
					m_unique.erase(itr);
					break;
				}
			}
			else if (libanim::Symbol* anim = dynamic_cast<libanim::Symbol*>(symbol))
			{
				bool prepared = true;
				for (size_t i = 0, n = anim->m_layers.size(); i < n && prepared; ++i)
				{
					libanim::Symbol::Layer* layer = anim->m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m && prepared; ++j)
					{
						libanim::Symbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l && prepared; ++k)
							if (!IsSymbolPrepared(frame->sprites[k]))
								prepared = false;
					}
				}
				if (prepared)
				{
					m_result.push_back(anim);
					m_unique.erase(itr);
					break;
				}
			}
			else if (escale9::Symbol* patch9 = dynamic_cast<escale9::Symbol*>(symbol))
			{
 				bool prepared = true;
				const escale9::Scale9Data& data = patch9->GetScale9Data();
 				switch (data.GetType())
 				{
 				case escale9::e_9Grid:
 					for (size_t i = 0; i < 3 && prepared; ++i)
 						for (size_t j = 0; j < 3 && prepared; ++j)
 							if (!IsSymbolPrepared(data.GetSprite(i, j)))
 								prepared = false;
 					break;
 				case escale9::e_9GridHollow:
 					for (size_t i = 0; i < 3 && prepared; ++i) {
 						for (size_t j = 0; j < 3 && prepared; ++j) {
 							if (i == 1 && j == 1) continue;
 							if (!IsSymbolPrepared(data.GetSprite(i, j)))
 								prepared = false;
 						}
 					}
 					break;
 				case escale9::e_3GridHor:
 					for (size_t i = 0; i < 3 && prepared; ++i)
 						if (!IsSymbolPrepared(data.GetSprite(1, i)))
 							prepared = false;
 					break;
 				case escale9::e_3GridVer:
 					for (size_t i = 0; i < 3 && prepared; ++i)
 						if (!IsSymbolPrepared(data.GetSprite(i, 1)))
 							prepared = false;
 					break;
 				case escale9::e_6GridUpper:
 					for (size_t i = 1; i < 3 && prepared; ++i)
 						for (size_t j = 0; j < 3 && prepared; ++j)
 							if (!IsSymbolPrepared(data.GetSprite(i, j)))
 								prepared = false;
 					break;
 				}
 				if (prepared)
 				{
 					m_result.push_back(patch9);
 					m_unique.erase(itr);
 					break;
 				}
			}
			else if (emesh::Symbol* mesh = dynamic_cast<emesh::Symbol*>(symbol))
			{
 				std::string path = mesh->GetImagePath();
 				d2d::ISymbol* image = d2d::SymbolMgr::Instance()->FetchSymbol(path);
				std::vector<const d2d::ISymbol*>::iterator itr_find 
					= std::find(m_result.begin(), m_result.end(), image);
				if (itr_find == m_result.end()) {
					m_result.push_back(image);
				}
				m_result.push_back(mesh);
				m_unique.erase(itr);
				break;
			}
			else if (eterrain2d::Symbol* ocean_symbol = dynamic_cast<eterrain2d::Symbol*>(symbol))
			{
				const std::vector<eterrain2d::OceanMesh*> oceans = ocean_symbol->GetOceans();
				assert(oceans.size() == 1);
				eterrain2d::OceanMesh* ocean = oceans[0];
				const d2d::ImageSymbol* img = ocean->GetImage0();
				std::string path = img->GetFilepath();

				d2d::ISymbol* image = d2d::SymbolMgr::Instance()->FetchSymbol(path);
				std::vector<const d2d::ISymbol*>::iterator itr_find 
					= std::find(m_result.begin(), m_result.end(), image);
				if (itr_find == m_result.end()) {
					m_result.push_back(image);
				}
				m_result.push_back(ocean_symbol);
				m_unique.erase(itr);
				break;
			}
			else if (etexture::Symbol* tex = dynamic_cast<etexture::Symbol*>(symbol))
			{
				const std::vector<d2d::IShape*>& shapes = tex->GetAllShapes();
				assert(shapes.size() == 1);
				for (int i = 0, n = shapes.size(); i < n; ++i)
				{
					d2d::IShape* shape = shapes[i];
					libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape);
					assert(poly);
					const libshape::TextureMaterial* material = dynamic_cast<const libshape::TextureMaterial*>(poly->GetMaterial());
					assert(material);
					const d2d::ImageSymbol* image = material->GetImage();
					std::vector<const d2d::ISymbol*>::iterator itr_find 
						= std::find(m_result.begin(), m_result.end(), image);
					if (itr_find == m_result.end()) {
						m_result.push_back(image);
					}
				}
				m_result.push_back(tex);
				m_unique.erase(itr);
				break;
			}
			else if (eicon::Symbol* icon = dynamic_cast<eicon::Symbol*>(symbol)) 
			{
				const std::string& filepath = icon->GetIcon()->GetImage()->GetFilepath();
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
				if (IsSymbolPrepared(symbol)) {
 					m_result.push_back(icon);
 					m_unique.erase(itr);
 					break;
				}

// 				m_result.push_back(icon);
// 				m_unique.erase(itr);
// 				break;
			}
		}
	}
}

bool SymbolDependanceSorter::IsSymbolPrepared(const d2d::ISprite* sprite) const
{
	return IsSymbolPrepared(&sprite->GetSymbol());
}

bool SymbolDependanceSorter::IsSymbolPrepared(const d2d::ISymbol* symbol) const
{
	for (size_t i = 0, n = m_result.size(); i < n; ++i) {
		if (symbol == m_result[i]) {
			return true;
		}
	}
	return false;	
}

void SymbolDependanceSorter::PrepareScale9(std::queue<const d2d::ISymbol*>& buffer,
										   const escale9::Symbol* scale9)
{
	if (m_unique.find(scale9) == m_unique.end())
	{
		m_unique.insert(scale9);
		const escale9::Scale9Data& data = scale9->GetScale9Data();
		switch (data.GetType())
		{
		case escale9::e_9Grid:
			for (size_t i = 0; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					buffer.push(&data.GetSprite(i, j)->GetSymbol());
			break;
		case escale9::e_9GridHollow:
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					if (i == 1 && j == 1) continue;
					buffer.push(&data.GetSprite(i, j)->GetSymbol());
				}
			}
			break;
		case escale9::e_3GridHor:
			for (size_t i = 0; i < 3; ++i)
				buffer.push(&data.GetSprite(1, i)->GetSymbol());
			break;
		case escale9::e_3GridVer:
			for (size_t i = 0; i < 3; ++i)
				buffer.push(&data.GetSprite(i, 1)->GetSymbol());
			break;
		case escale9::e_6GridUpper:
			for (size_t i = 1; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					buffer.push(&data.GetSprite(i, j)->GetSymbol());
			break;
		}
	}
}

}