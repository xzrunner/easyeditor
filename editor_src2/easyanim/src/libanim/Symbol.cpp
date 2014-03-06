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

void Symbol::draw(const d2d::ISprite* sprite/* = NULL*/) const
{
	int index = d2d::SpriteDraw::time / (1.0f / m_fps);
	index = index % getMaxFrameIndex();

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
					d2d::SpriteDraw::drawSprite(sprite);
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
	d2d::AnimFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	m_name = name = adapter.name;

	m_fps = adapter.fps;

	std::string dlg = d2d::FilenameTools::getFileDir(m_filepath);

	for (size_t i = 0, n = adapter.layers.size(); i < n; ++i)
	{
		Layer* dstLayer = new Layer;
		d2d::AnimFileAdapter::Layer* srcLayer = adapter.layers[i];
		dstLayer->name = srcLayer->name;
		for (size_t j = 0, m = srcLayer->frames.size(); j < m; ++j)
		{
			Frame* dstFrame = new Frame;
			d2d::AnimFileAdapter::Frame* srcFrame = srcLayer->frames[j];
			//			dstFrame->id = srcFrame->id;
			dstFrame->index = srcFrame->index;
			dstFrame->bClassicTween = srcFrame->bClassicTween;
			for (size_t k = 0, l = srcFrame->entries.size(); k < l; ++k)
			{
				d2d::AnimFileAdapter::Entry* entry = srcFrame->entries[k];

				std::string filepath = entry->filepath;
				if (!d2d::FilenameTools::isExist(filepath))
					filepath = d2d::FilenameTools::getAbsolutePath(dlg, filepath);

				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
				d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);

				sprite->name = entry->name;
				if (entry->multiColor.empty())
					sprite->multiCol = d2d::Colorf(1, 1, 1, 1);
				else
					sprite->multiCol = transColor(entry->multiColor, d2d::PT_BGRA);
				if (entry->addColor.empty())
					sprite->addCol = d2d::Colorf(0 ,0, 0, 0);
				else
					sprite->addCol = transColor(entry->addColor, d2d::PT_ARGB);

				sprite->setTransform(entry->pos, entry->angle);
				sprite->setScale(entry->xScale, entry->yScale);
				sprite->setShear(entry->xShear, entry->yShear);
				sprite->setOffset(d2d::Vector(entry->xOffset, entry->yOffset));
				sprite->setMirror(entry->xMirror, entry->yMirror);

				dstFrame->sprites.push_back(sprite);
			}
			dstLayer->frames.push_back(dstFrame);
		}
		m_layers.push_back(dstLayer);
	}

	initBounding();
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