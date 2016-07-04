#include "Symbol.h"
#include "config.h"
#include "Utility.h"
#include "LayersLoader.h"

#include <ee/BoundingBox.h>
#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/Visitor.h>

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace eanim
{

Symbol::Symbol()
	: m_index(0)
	, m_init_time(0)
	, m_loop(true)
{
	this->Retain();
	m_core = new s2::AnimSymbol(this);

	static int id = 0;
	m_name = eanim::FILE_TAG + wxVariant(id++);
}

Symbol::~Symbol()
{
	this->Release();
	delete m_core;

	Clear();
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	if (m_index != 0) {
		Utility::DrawAnimSymbol(this, p, m_index);
	} else {
		if (m_init_time == 0) {
			m_init_time = clock();
			Utility::DrawAnimSymbol(this, p, 1);
		} else {
			Utility::DrawAnimSymbol(this, p, GetCurrFrame());
		}
	}
}

void Symbol::ReloadTexture() const
{
	const std::vector<s2::AnimSymbol::Layer*>& layers = m_core->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j)
		{
			s2::AnimSymbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprites.size(); k < l; ++k) {
				ee::Sprite* spr = static_cast<ee::Sprite*>(frame->sprites[k]->GetUD());
				spr->GetSymbol().ReloadTexture();
			}
		}
	}
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite/* = NULL*/) const
{
	return m_rect;
}

void Symbol::Traverse(ee::Visitor& visitor)
{
	const std::vector<s2::AnimSymbol::Layer*>& layers = m_core->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j)
		{
			s2::AnimSymbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprites.size(); k < l; ++k) {
				ee::Sprite* spr = static_cast<ee::Sprite*>(frame->sprites[k]->GetUD());
				bool next;
				visitor.Visit(spr, next);
				const_cast<ee::Symbol&>(spr->GetSymbol()).Traverse(visitor);
			}
		}
	}
}

size_t Symbol::getMaxFrameIndex() const
{
	int index = 0;
	const std::vector<s2::AnimSymbol::Layer*>& layers = m_core->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			index = std::max(index, layer->frames[j]->index);
		}
	}
	return index;
}

void Symbol::InitBounding()
{
	m_rect.MakeEmpty();
	const std::vector<s2::AnimSymbol::Layer*>& layers = m_core->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = layers[i];
		for (int i = 0, n = layer->frames.size(); i < n; ++i)
		{
			s2::AnimSymbol::Frame* frame = layer->frames[i];
			for (int i = 0, n = frame->sprites.size(); i < n; ++i)
			{
				std::vector<sm::vec2> vertices;
				ee::Sprite* spr = static_cast<ee::Sprite*>(frame->sprites[i]->GetUD());
				spr->GetBounding()->GetBoundPos(vertices);
				for (int i = 0, n = vertices.size(); i < n; ++i) {
					m_rect.Combine(vertices[i]);
				}
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
		m_core->AddLayer(layers[i]);
	}

	InitBounding();
}

int Symbol::GetCurrFrame() const
{
	clock_t curr = clock();
	float during = (float)(curr - m_init_time) / CLOCKS_PER_SEC;
	int index = during / (1.0f / m_fps);
	int max_frame = getMaxFrameIndex();
	if (!m_loop && index > max_frame) {
		return 1;
	} else {
		return index % max_frame + 1;
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

void Symbol::Clear()
{
	m_core->Clear();

	m_index = 0;

	const std::vector<s2::AnimSymbol::Layer*>& layers = m_core->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		s2::AnimSymbol::Layer* layer = layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			s2::AnimSymbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprites.size(); k < l; ++k) {
				ee::Sprite* spr = static_cast<ee::Sprite*>(frame->sprites[k]->GetUD());
				spr->Release();
			}
		}
	}
}

}