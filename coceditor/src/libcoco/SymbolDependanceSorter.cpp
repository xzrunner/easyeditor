#include "SymbolDependanceSorter.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>

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
		symbols.push_back(&sprites[i]->getSymbol());
	prepare(symbols);
}

void SymbolDependanceSorter::fetch(const std::vector<const d2d::ISymbol*>& symbols)
{
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
		m_unique.insert(symbols[i]);

	std::queue<d2d::ISprite*> buffer;
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		const d2d::ISymbol* symbol = symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol))
		{
			for (size_t j = 0, n = complex->m_sprites.size(); j < n; ++j)
				buffer.push(complex->m_sprites[j]);
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
						buffer.push(frame->sprites[k]);
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
 						buffer.push(data.GetSprite(i, j));
 				break;
 			case escale9::e_9GridHollow:
 				for (size_t i = 0; i < 3; ++i) {
 					for (size_t j = 0; j < 3; ++j) {
 						if (i == 1 && j == 1) continue;
 						buffer.push(data.GetSprite(i, j));
 					}
 				}
 				break;
 			case escale9::e_3GridHor:
 				for (size_t i = 0; i < 3; ++i)
 					buffer.push(data.GetSprite(1, i));
 				break;
 			case escale9::e_3GridVer:
 				for (size_t i = 0; i < 3; ++i)
 					buffer.push(data.GetSprite(i, 1));
 				break;
 			case escale9::e_6GridUpper:
 				for (size_t i = 1; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j)
 						buffer.push(data.GetSprite(i, j));
 				break;
 			}
		}
	}

	while (!buffer.empty())
	{
		d2d::ISprite* sprite = buffer.front(); buffer.pop();
		if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(sprite))
		{
			m_unique.insert(&sprite->getSymbol());
		}
		else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(sprite))
		{
			m_unique.insert(&sprite->getSymbol());
		}
		else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(sprite))
		{
			const ecomplex::Symbol& symbol = complex->getSymbol();
			if (m_unique.find(&symbol) == m_unique.end())
			{
				m_unique.insert(&symbol);
				for (size_t i = 0, n = symbol.m_sprites.size(); i < n; ++i)
				{
					d2d::ISprite* child = symbol.m_sprites[i];
					buffer.push(child);

					// patch for scale9
					if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(child)) {
						PrepareScale9(buffer, scale9);
					}
				}
			}
		}
		else if (const libanim::Sprite* anim = dynamic_cast<const libanim::Sprite*>(sprite))
		{
			const libanim::Symbol& symbol = anim->getSymbol();
			if (m_unique.find(&symbol) == m_unique.end())
			{
				m_unique.insert(&symbol);
				for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
				{
					libanim::Symbol::Layer* layer = symbol.m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
					{
						libanim::Symbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						{
							d2d::ISprite* child = frame->sprites[k];
							buffer.push(child);

							// patch for scale9
							if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(child)) {
								PrepareScale9(buffer, scale9);
							}
						}
					}
				}
			}
		}
		else if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(sprite))
		{
			PrepareScale9(buffer, scale9);
		}
		else if (const emesh::Sprite* mesh = dynamic_cast<const emesh::Sprite*>(sprite))
		{
			m_unique.insert(&mesh->getSymbol());
		}
		else if (const eterrain2d::Sprite* ocean = dynamic_cast<const eterrain2d::Sprite*>(sprite))
		{
			m_unique.insert(&ocean->getSymbol());
		}
		else if (const etexture::Sprite* tex = dynamic_cast<const etexture::Sprite*>(sprite))
		{
			m_unique.insert(&tex->getSymbol());
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
				std::string path = symbol->getFilepath();
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
					if (!isSymbolPrepared(complex->m_sprites[i])) {
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
							if (!isSymbolPrepared(frame->sprites[k]))
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
 							if (!isSymbolPrepared(data.GetSprite(i, j)))
 								prepared = false;
 					break;
 				case escale9::e_9GridHollow:
 					for (size_t i = 0; i < 3 && prepared; ++i) {
 						for (size_t j = 0; j < 3 && prepared; ++j) {
 							if (i == 1 && j == 1) continue;
 							if (!isSymbolPrepared(data.GetSprite(i, j)))
 								prepared = false;
 						}
 					}
 					break;
 				case escale9::e_3GridHor:
 					for (size_t i = 0; i < 3 && prepared; ++i)
 						if (!isSymbolPrepared(data.GetSprite(1, i)))
 							prepared = false;
 					break;
 				case escale9::e_3GridVer:
 					for (size_t i = 0; i < 3 && prepared; ++i)
 						if (!isSymbolPrepared(data.GetSprite(i, 1)))
 							prepared = false;
 					break;
 				case escale9::e_6GridUpper:
 					for (size_t i = 1; i < 3 && prepared; ++i)
 						for (size_t j = 0; j < 3 && prepared; ++j)
 							if (!isSymbolPrepared(data.GetSprite(i, j)))
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
 				d2d::ISymbol* image = d2d::SymbolMgr::Instance()->fetchSymbol(path);
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
				std::string path = img->getFilepath();

				d2d::ISymbol* image = d2d::SymbolMgr::Instance()->fetchSymbol(path);
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
		}
	}
}

bool SymbolDependanceSorter::isSymbolPrepared(const d2d::ISprite* sprite) const
{
	for (size_t i = 0, n = m_result.size(); i < n; ++i)
		if (&sprite->getSymbol() == m_result[i])
			return true;
	return false;
}

void SymbolDependanceSorter::PrepareScale9(std::queue<d2d::ISprite*>& buffer,
										   const escale9::Sprite* scale9)
{
	const escale9::Symbol& symbol = scale9->getSymbol();
	if (m_unique.find(&symbol) == m_unique.end())
	{
		m_unique.insert(&symbol);
		const escale9::Scale9Data& data = symbol.GetScale9Data();
		switch (data.GetType())
		{
		case escale9::e_9Grid:
			for (size_t i = 0; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					buffer.push(data.GetSprite(i, j));
			break;
		case escale9::e_9GridHollow:
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					if (i == 1 && j == 1) continue;
					buffer.push(data.GetSprite(i, j));
				}
			}
			break;
		case escale9::e_3GridHor:
			for (size_t i = 0; i < 3; ++i)
				buffer.push(data.GetSprite(1, i));
			break;
		case escale9::e_3GridVer:
			for (size_t i = 0; i < 3; ++i)
				buffer.push(data.GetSprite(i, 1));
			break;
		case escale9::e_6GridUpper:
			for (size_t i = 1; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					buffer.push(data.GetSprite(i, j));
			break;
		}
	}
}

}