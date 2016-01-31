#include "StagePanel.h"

#include "StageCanvas.h"
#include "Context.h"
#include "SelectSpritesOP.h"
#include "TreeCtrl.h"

#include <easycomplex.h>
#include <easyanim.h>

using namespace edb;

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(GetStageImpl())
{
	m_edit_op = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, this);
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
}

void StagePanel::loadFromDir(const std::string& dirpath)
{
	ee::ClearSpriteSJ::Instance()->Clear();

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dirpath, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = files[i].ToStdString();
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)
			|| ee::FileType::IsType(filepath, ee::FileType::e_anim))
		{
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
			symbol->Release();
			ee::InsertSpriteSJ::Insert(sprite);
		}
	}

	initConnection();
	initPosition();

	Context::Instance()->tree->init(m_graphics);
}

void StagePanel::loadFromDirFast(const std::string& dirpath)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dirpath, files);

	Context::Instance()->tree->init(files);
}

void StagePanel::initConnection()
{
	m_graphics.clear();

	std::vector<ee::Sprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* from = sprites[i];
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(from))
		{
			const ecomplex::Symbol& symbol = complex->GetSymbol();
			for (size_t i = 0, n = symbol.m_sprites.size(); i < n; ++i)
			{
				ee::Sprite* child = symbol.m_sprites[i];
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
						ee::Sprite* child = frame->sprites[i];
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
	std::vector<ee::Sprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	// 1 single random
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];

		Node* node = m_graphics.query(sprite);
		if (!node || node->in.empty())
		{
			ee::Vector pos;
			float radius = ee::Random::GetNum0To1() * 4096;
			float angle = ee::Random::GetNum(0, ee::PI*2);
			pos.x = cos(angle)*radius;
			pos.y = sin(angle)*radius;
			sprite->SetTransform(pos, 0);
		}
	}

	// 2 to
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];

		Node* node = m_graphics.query(sprite);
//		if (node && node->in.empty())
		if (node)
		{
			float radius = 256;
			float angle = ee::PI*2 / node->out.size();
			for (size_t i = 0, n = node->out.size(); i < n; ++i)
			{
				ee::Sprite* to = node->out[i];

				ee::Vector pos = sprite->GetPosition();
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