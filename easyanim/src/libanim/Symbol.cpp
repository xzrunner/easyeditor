#include "Symbol.h"
#include "config.h"
#include "LayersLoader.h"

#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/Visitor.h>

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace eanim
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = eanim::FILE_TAG + wxVariant(id++);
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

void Symbol::LoadFromFile(const LayersLoader& loader)
{
	Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_name  = name = value["name"].asString();
	m_fps = value["fps"].asInt();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);

	std::vector<s2::AnimSymbol::Layer*> layers;
	loader.LoadLayers(value, dir, layers);
	for (int i = 0, n = layers.size(); i < n; ++i) {
		AddLayer(layers[i]);
	}
}

void Symbol::LoadResources()
{
	class Loader : public LayersLoader
	{
	protected:
		virtual std::string GetSymbolPath(const std::string& dir, 
			const Json::Value& json_val) const
		{
			return ee::SymbolSearcher::GetSymbolPath(dir, json_val);
		}
	};

	Loader loader;
	LoadFromFile(loader);
}

}