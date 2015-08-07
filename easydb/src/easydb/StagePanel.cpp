#include "StagePanel.h"

#include "StageCanvas.h"
#include "Context.h"
#include "SelectSpritesOP.h"
#include "TreeCtrl.h"

#include <easycomplex.h>
#include <easyanim.h>

using namespace edb;

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, d2d::ShapesPanelImpl(GetStageImpl())
{
	m_edit_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this);
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	Clear();
}

void StagePanel::Clear()
{
	d2d::EditPanel::Clear();
	d2d::SpritesPanelImpl::ClearAllSprite();
	d2d::ShapesPanelImpl::ClearAllShapes();
}

void StagePanel::loadFromDir(const std::string& dirpath)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	ClearAllSprite();
	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = files[i].ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
			|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			symbol->Release();
			InsertSprite(sprite);
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
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* from = sprites[i];
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(from))
		{
			const ecomplex::Symbol& symbol = complex->GetSymbol();
			for (size_t i = 0, n = symbol.m_sprites.size(); i < n; ++i)
			{
				d2d::ISprite* child = symbol.m_sprites[i];
				for (size_t i = 0, n = sprites.size(); i < n; ++i)
					if (&child->GetSymbol() == &sprites[i]->GetSymbol())
						m_graphics.connect(from, sprites[i]);
			}
		}
		else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(from))
		{
			const libanim::Symbol& symbol = anim->GetSymbol();
			for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
			{
				libanim::Symbol::Layer* layer = symbol.m_layers[i];
				for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
				{
					libanim::Symbol::Frame* frame = layer->frames[i];
					for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
					{
						d2d::ISprite* child = frame->sprites[i];
						for (size_t i = 0, n = sprites.size(); i < n; ++i)
							if (&child->GetSymbol() == &sprites[i]->GetSymbol())
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
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

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
			sprite->SetTransform(pos, 0);
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

				d2d::Vector pos = sprite->GetPosition();
				pos.x += cos(angle*i)*radius;
				pos.y += sin(angle*i)*radius;
// 				if (to->getPosition().x != 0 || to->getPosition().y != 0)
// 					pos = (pos + to->getPosition()) * 0.5f;

			//	to->setTransform(pos, 0);
				m_graphics.move(to, pos - to->GetPosition());
			}
		}
	}
}