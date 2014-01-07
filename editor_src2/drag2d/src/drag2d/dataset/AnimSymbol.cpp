#include "AnimSymbol.h"
#include "ISprite.h"
#include "AnimSprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/AnimFileAdapter.h"
#include "common/FileNameTools.h"
#include "dataset/Bitmap.h"
#include "dataset/AbstractBV.h"
#include "dataset/ImageSprite.h"
#include "dataset/ComplexSprite.h"
#include "render/SpriteDraw.h"

namespace d2d
{

AnimSymbol::AnimSymbol()
{
	static int id = 0;
	m_name = wxT("anim") + wxVariant(id++);

	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;
	m_bitmap = new Bitmap(
		new wxBitmap(WIDTH * SCALE, HEIGHT * SCALE)
		);
}

AnimSymbol::~AnimSymbol()
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

void AnimSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void AnimSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void AnimSymbol::reloadTexture() const
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

void AnimSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	int index = SpriteDraw::time / (1.0f / m_fps);
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
					SpriteDraw::drawSprite(sprite);
				}
			}
		}
	}
}

float AnimSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float AnimSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void AnimSymbol::refresh()
{
	ISymbol::refresh();
	refreshThumbnail();
	initBounding();
}

size_t AnimSymbol::getMaxFrameIndex() const
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

void AnimSymbol::loadResources()
{
	AnimFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	m_name = name = adapter.name;

	m_fps = adapter.fps;

	std::string dlg = FilenameTools::getFileDir(m_filepath);

	for (size_t i = 0, n = adapter.layers.size(); i < n; ++i)
	{
		Layer* dstLayer = new Layer;
		AnimFileAdapter::Layer* srcLayer = adapter.layers[i];
		dstLayer->name = srcLayer->name;
		for (size_t j = 0, m = srcLayer->frames.size(); j < m; ++j)
		{
			Frame* dstFrame = new Frame;
			AnimFileAdapter::Frame* srcFrame = srcLayer->frames[j];
			dstFrame->id = srcFrame->id;
			dstFrame->index = srcFrame->index;
			dstFrame->bClassicTween = srcFrame->bClassicTween;
			for (size_t k = 0, l = srcFrame->entries.size(); k < l; ++k)
			{
				AnimFileAdapter::Entry* entry = srcFrame->entries[k];
				
				std::string filepath = entry->filepath;
				if (!FilenameTools::isExist(filepath))
					filepath = FilenameTools::getAbsolutePath(dlg, filepath);

				ISymbol* symbol = SymbolMgr::Instance()->getSymbol(filepath);
				ISprite* sprite = SpriteFactory::Instance()->create(symbol);

				sprite->name = entry->name;
				sprite->multiColor = entry->multiColor;
				sprite->addColor = entry->addColor;

				sprite->setTransform(entry->pos, entry->angle);
				sprite->setScale(entry->xScale, entry->yScale);
				sprite->setShear(entry->xShear, entry->yShear);
				sprite->setMirror(entry->xMirror, entry->yMirror);

				dstFrame->sprites.push_back(sprite);
			}
			dstLayer->frames.push_back(dstFrame);
		}
		m_layers.push_back(dstLayer);
	}

	initBounding();
}

void AnimSymbol::initBounding()
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
				std::vector<Vector> vertices;
				frame->sprites[j]->getBounding()->getBoundPos(vertices);
				for (size_t k = 0, l = vertices.size(); k < l; ++k)
					m_rect.combine(vertices[k]);
			}
		}
	}
}

void AnimSymbol::refreshThumbnail()
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
				ISprite* sprite = frame->sprites[j];
				if (ImageSprite* image = dynamic_cast<ImageSprite*>(sprite))
					SpriteDraw::drawSprite(image, memDC);
				else if (ComplexSprite* complex = dynamic_cast<ComplexSprite*>(sprite))
				{
					const Vector& offset = complex->getPosition();
					std::vector<std::pair<const ISprite*, Vector> > children;
					complex->getSymbol().getAllChildren(children);
					for (size_t k = 0, l = children.size(); k < l; ++k)
						SpriteDraw::drawSprite(children[k].first, children[k].second + offset, memDC);
				}
			}
		}
	}

	memDC.SelectObject(wxNullBitmap);
}

//////////////////////////////////////////////////////////////////////////
// class AnimSymbol::Layer
//////////////////////////////////////////////////////////////////////////

AnimSymbol::Frame* AnimSymbol::Layer::getCurrFrame(int index) const
{
	d2d::AnimSymbol::Frame* ret = NULL;

	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		d2d::AnimSymbol::Frame* frame = frames[i];
		if (frame->index <= index)
			ret = frame;
		else
			break;
	}
	return ret;
}

AnimSymbol::Frame* AnimSymbol::Layer::getNextFrame(int index) const
{
	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		d2d::AnimSymbol::Frame* frame = frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}

} // d2d