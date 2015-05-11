#include "Symbol.h"
#include "config.h"
#include "Tools.h"

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

void Symbol::reloadTexture() const
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
		{
			Frame* frame = layer->frames[j];
			for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
				frame->sprites[k]->getSymbol().reloadTexture();
		}
	}
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	if (m_index != 0) {
		Tools::drawAnimSymbol(this, mt, m_index, mul, add, r_trans, g_trans, b_trans);
	} else {
		static clock_t init = 0;
		if (init == 0) {
			init = clock();
			Tools::drawAnimSymbol(this, mt, 1, mul, add, r_trans, g_trans, b_trans);
		} else {
			clock_t curr = clock();
			float during = (float)(curr - init) / CLOCKS_PER_SEC;
			int index = during / (1.0f / m_fps);
			Tools::drawAnimSymbol(this, mt, index % getMaxFrameIndex() + 1, mul, add, r_trans, g_trans, b_trans);
		}
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
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

void Symbol::loadResources()
{
	clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(m_filepath);

	m_name  = name = value["name"].asString();
	m_fps = value["fps"].asInt();

	// layers
	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* dstLayer = new Layer;
		dstLayer->name = layerValue["name"].asString();
		// frames
		int j = 0;
		Json::Value frameValue = layerValue["frame"][j++];
		while (!frameValue.isNull()) {
			Frame* dstFrame = new Frame;
			//dstFrame->id = frameValue["id"].asInt();
			dstFrame->index = frameValue["time"].asInt();
			dstFrame->bClassicTween = frameValue["tween"].asBool();
			// sprites
			int k = 0;
			Json::Value spriteValue = frameValue["actor"][k++];
			while (!spriteValue.isNull()) {
				wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spriteValue);
				if (!d2d::FilenameTools::isExist(filepath)) {
					throw d2d::Exception("Symbol doesn't exist: %s !", spriteValue["filepath"].asString().c_str());
				}
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
				d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spriteValue);

				d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
				symbol->Release();
				sprite->load(spriteValue);
				dstFrame->sprites.push_back(sprite);
				spriteValue = frameValue["actor"][k++];
			}
			dstLayer->frames.push_back(dstFrame);
			frameValue = layerValue["frame"][j++];
		}
		m_layers.push_back(dstLayer);
		layerValue = value["layer"][i++];
	}

	initBounding();
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

void Symbol::initBounding()
{
	m_rect.makeInfinite();
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		for (int i = 0, n = layer->frames.size(); i < n; ++i)
		{
			Frame* frame = layer->frames[i];
			for (int i = 0, n = frame->sprites.size(); i < n; ++i)
			{
				std::vector<d2d::Vector> vertices;
				frame->sprites[i]->getBounding()->getBoundPos(vertices);
				for (int i = 0, n = vertices.size(); i < n; ++i) {
					m_rect.combine(vertices[i]);
				}
			}
		}
	}
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