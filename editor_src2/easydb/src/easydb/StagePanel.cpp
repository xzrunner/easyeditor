#include "StagePanel.h"

#include "StageCanvas.h"
#include "Context.h"
#include "SelectSpritesOP.h"
#include "TreeCtrl.h"

#include <easycomplex.h>

using namespace edb;

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, NULL)
	, d2d::ShapesPanelImpl(this)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this);
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	d2d::SpritesPanelImpl::clearSprites();
	d2d::ShapesPanelImpl::clearShapes();
}

void StagePanel::loadFromDir(const std::string& dirpath)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	clearSprites();
	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		wxString filepath = files[i];
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
			|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			insertSprite(sprite);
		}
	}

	initConnection();
	initPosition();

	Context::Instance()->tree->init(m_graphics);
}

void StagePanel::loadFromDirFast(const std::string& dirpath)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	Context::Instance()->tree->init(files);
}

void StagePanel::initConnection()
{
	m_graphics.clear();

	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* from = sprites[i];
		if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(from))
		{
			const complex::Symbol& symbol = complex->getSymbol();
			for (size_t i = 0, n = symbol.m_sprites.size(); i < n; ++i)
			{
				d2d::ISprite* child = symbol.m_sprites[i];
				for (size_t i = 0, n = sprites.size(); i < n; ++i)
					if (&child->getSymbol() == &sprites[i]->getSymbol())
						m_graphics.connect(from, sprites[i]);
			}
		}
		else if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(from))
		{
			const d2d::AnimSymbol& symbol = anim->getSymbol();
			for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
			{
				d2d::AnimSymbol::Layer* layer = symbol.m_layers[i];
				for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
				{
					d2d::AnimSymbol::Frame* frame = layer->frames[i];
					for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
					{
						d2d::ISprite* child = frame->sprites[i];
						for (size_t i = 0, n = sprites.size(); i < n; ++i)
							if (&child->getSymbol() == &sprites[i]->getSymbol())
								m_graphics.connect(from, sprites[i]);
					}
				}
			}
		}
	}
}

void StagePanel::initPosition()
{
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	// 1 single random
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		Node* node = m_graphics.query(sprite);
		if (!node || node->in.empty())
		{
			d2d::Vector pos;
			float radius = d2d::Random::getNum0To1() * 4096;
			float angle = d2d::Random::getNum(0, d2d::PI*2);
			pos.x = cos(angle)*radius;
			pos.y = sin(angle)*radius;
			sprite->setTransform(pos, 0);
		}
	}

	// 2 to
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		Node* node = m_graphics.query(sprite);
//		if (node && node->in.empty())
		if (node)
		{
			float radius = 256;
			float angle = d2d::PI*2 / node->out.size();
			for (size_t i = 0, n = node->out.size(); i < n; ++i)
			{
				d2d::ISprite* to = node->out[i];

				d2d::Vector pos = sprite->getPosition();
				pos.x += cos(angle*i)*radius;
				pos.y += sin(angle*i)*radius;
// 				if (to->getPosition().x != 0 || to->getPosition().y != 0)
// 					pos = (pos + to->getPosition()) * 0.5f;

			//	to->setTransform(pos, 0);
				m_graphics.move(to, pos - to->getPosition());
			}
		}
	}
}