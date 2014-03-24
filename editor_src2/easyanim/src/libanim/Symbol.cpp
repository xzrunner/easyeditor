#include "Symbol.h"

#include <easycomplex.h>

namespace anim
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = wxT("anim") + wxVariant(id++);

	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;
	m_bitmap = new d2d::Bitmap(
		new wxBitmap(WIDTH * SCALE, HEIGHT * SCALE)
		);
}

Symbol::~Symbol()
{
	clear();
}

void Symbol::loadFromTextFile(std::ifstream& fin)
{
}

void Symbol::storeToTextFile(std::ofstream& fout) const
{
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

void Symbol::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	int index = d2d::SpriteDraw::time / (1.0f / m_fps);
	index = (index + 1) % getMaxFrameIndex();

	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (!layer->frames.empty())
		{
			Frame* frame = NULL;
			for (int i = layer->frames.size() - 1; i >= 0; --i)
			{
				if (layer->frames[i]->index <= index)
				{
					frame = layer->frames[i];
					break;
				}
			}

			if (frame)
			{
				for (size_t j = 0, m = frame->sprites.size(); j < m; ++j)
				{
					d2d::ISprite* sprite = frame->sprites[j];
					d2d::SpriteDraw::drawSprite(sprite, mul, add);
				}
			}
		}
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return m_rect;
}

void Symbol::refresh()
{
	d2d::ISymbol::refresh();
	refreshThumbnail();
	initBounding();
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
	std::ifstream fin(m_filepath.fn_str());
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
 				wxString path = d2d::FilenameTools::getAbsolutePath(dir, spriteValue["filepath"].asString());
 				ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
				d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
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
				frame->sprites[k]->release();
			delete frame;
		}
		delete layer;
	}
}

void Symbol::initBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (!layer->frames.empty())
		{
			Frame* frame = layer->frames[0];
			for (size_t j = 0, m = frame->sprites.size(); j < m; ++j)
			{
				std::vector<d2d::Vector> vertices;
				frame->sprites[j]->getBounding()->getBoundPos(vertices);
				for (size_t k = 0, l = vertices.size(); k < l; ++k)
					m_rect.combine(vertices[k]);

				// 				d2d::ISprite* sprite = frame->sprites[j];
				// 				float hw = sprite->getSymbol().getSize().xLength() * 0.5f;
				// 				float hh = sprite->getSymbol().getSize().yLength() * 0.5f;
				// 				love::Matrix t;
				// 				t.setTransformation(sprite->getPosition().x, sprite->getPosition().y, sprite->getAngle(),
				// 					sprite->getScale().x, sprite->getScale().y, 0, 0, sprite->getShear().x, sprite->getShear().y);
				// 				m_rect.combine(Math::transVector(d2d::Vector(-hw, -hh), t));
				// 				m_rect.combine(Math::transVector(d2d::Vector(-hw,  hh), t));
				// 				m_rect.combine(Math::transVector(d2d::Vector( hw,  hh), t));
				// 				m_rect.combine(Math::transVector(d2d::Vector( hw, -hh), t));
			}
		}
	}
}

void Symbol::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxWHITE));
	memDC.Clear();

	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (!layer->frames.empty())
		{
			Frame* frame = layer->frames[0];
			for (size_t j = 0, m = frame->sprites.size(); j < m; ++j)
			{
				d2d::ISprite* sprite = frame->sprites[j];
				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
					d2d::SpriteDraw::drawSprite(image, memDC);
				else if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(sprite))
				{
					const d2d::Vector& offset = complex->getPosition();
					std::vector<std::pair<const d2d::ISprite*, d2d::Vector> > children;
					complex->getSymbol().getAllChildren(children);
					for (size_t k = 0, l = children.size(); k < l; ++k)
						d2d::SpriteDraw::drawSprite(children[k].first, children[k].second + offset, memDC);
				}
			}
		}
	}

	memDC.SelectObject(wxNullBitmap);
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