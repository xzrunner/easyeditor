#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/Visitor.h>
#include <ee/SpriteLoader.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>

#include <sprite2/SymType.h>
#include <sprite2/S2_Sprite.h>
#include <gum/AnimSymLoader.h>
#include <gum/StringHelper.h>

#include <fstream>

namespace eanim
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + gum::StringHelper::ToString(id++);
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j)
		{
			s2::AnimSymbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				s2::Symbol* sym = frame->sprs[k]->GetSymbol();
				dynamic_cast<ee::Symbol*>(sym)->ReloadTexture();
			}
		}
	}
}

void Symbol::Traverse(ee::Visitor<ee::Sprite>& visitor)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j)
		{
			s2::AnimSymbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				ee::Sprite* spr = dynamic_cast<ee::Sprite*>(frame->sprs[k]);
				bool next;
				visitor.Visit(spr, next);
				dynamic_cast<ee::Symbol*>(spr->GetSymbol())->Traverse(visitor);
			}
		}
	}
}

void Symbol::Load(const gum::SpriteLoader& spr_loader)
{
	Clear();

	gum::AnimSymLoader loader(this, &spr_loader);
	loader.LoadJson(m_filepath);
}

void Symbol::LoadResources()
{
	ee::SpriteLoader spr_loader;
	gum::AnimSymLoader loader(this, &spr_loader);
	loader.LoadJson(m_filepath);
}

}