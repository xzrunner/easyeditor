#include "Symbol.h"
#include "config.h"
#include "Utility.h"
#include "LayersLoader.h"

#include <easycomplex.h>

namespace libanim
{

Symbol::Symbol()
	: m_index(0)
{
	static int id = 0;
	m_name = libanim::FILE_TAG + wxVariant(id++);
}

Symbol::~Symbol()
{
	clear();
}

void Symbol::ReloadTexture() const
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
		{
			Frame* frame = layer->frames[j];
			for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
				frame->sprites[k]->GetSymbol().ReloadTexture();
		}
	}
}

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	if (m_index != 0) {
		Utility::DrawAnimSymbol(this, mt, m_index, color);
	} else {
		static clock_t init = 0;
		if (init == 0) {
			init = clock();
			Utility::DrawAnimSymbol(this, mt, 1, color);
		} else {
			clock_t curr = clock();
			float during = (float)(curr - init) / CLOCKS_PER_SEC;
			int index = during / (1.0f / m_fps);
			Utility::DrawAnimSymbol(this, mt, index % getMaxFrameIndex() + 1, color);
		}
	}
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite/* = NULL*/) const
{
	return m_rect;
}

size_t Symbol::getMaxFrameIndex() const
{
	int index = 0;
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
			index = std::max(index, layer->frames[j]->index);
	}
	return index;
}

void Symbol::InitBounding()
{
	m_rect.MakeInfinite();
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		for (int i = 0, n = layer->frames.size(); i < n; ++i)
		{
			Frame* frame = layer->frames[i];
			for (int i = 0, n = frame->sprites.size(); i < n; ++i)
			{
				std::vector<ee::Vector> vertices;
				frame->sprites[i]->GetBounding()->GetBoundPos(vertices);
				for (int i = 0, n = vertices.size(); i < n; ++i) {
					m_rect.Combine(vertices[i]);
				}
			}
		}
	}
}

void Symbol::LoadFromFile(const LayersLoader& loader)
{
	clear();

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

	loader.LoadLayers(value, dir, m_layers);

	InitBounding();
}

void Symbol::LoadResources()
{
	class Loader : public LayersLoader
	{
	protected:
		virtual std::string GetSymbolPath(const std::string& dir, 
			const Json::Value& json_val) const
		{
			return ee::SymbolSearcher::GetSymbolPath(dir, json_val).ToStdString();
		}
	};

	Loader loader;
	LoadFromFile(loader);
}

void Symbol::clear()
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
		{
			Frame* frame = layer->frames[j];
			for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
				frame->sprites[k]->Release();
			delete frame;
		}
		delete layer;
	}
	m_layers.clear();

	m_index = 0;
}

//////////////////////////////////////////////////////////////////////////
// class Symbol::Layer
//////////////////////////////////////////////////////////////////////////

Symbol::Frame* Symbol::Layer::getCurrFrame(int index) const
{
	Symbol::Frame* ret = NULL;

	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		Symbol::Frame* frame = frames[i];
		if (frame->index <= index)
			ret = frame;
		else
			break;
	}
	return ret;
}

Symbol::Frame* Symbol::Layer::getNextFrame(int index) const
{
	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		Symbol::Frame* frame = frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}

}