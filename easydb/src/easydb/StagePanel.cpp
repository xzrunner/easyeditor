#include "StagePanel.h"

#include "StageCanvas.h"
#include "Context.h"
#include "SelectSpritesOP.h"
#include "TreeCtrl.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/sprite_msg.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/FetchAllVisitor.h>
#include <ee/sprite_msg.h>
#include <ee/Random.h>

#include <easycomplex.h>
#include <easyanim.h>

#include <sprite2/S2_Sprite.h>

namespace edb
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl()
{
	ee::EditOP* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, this->GetStageImpl(), this);
	SetEditOP(editop);
	editop->Release();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->Release();
}

StagePanel::~StagePanel()
{
}

void StagePanel::LoadFromDir(const std::string& dirpath)
{
	ee::ClearSpriteSJ::Instance()->Clear();

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dirpath, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = files[i].ToStdString();
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex) || 
			ee::FileType::IsType(filepath, ee::FileType::e_anim))
		{
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
			symbol->Release();
			ee::InsertSpriteSJ::Instance()->Insert(sprite);
		}
	}

	InitConnection();
	InitPosition();

	Context::Instance()->tree->init(m_graphics);
}

void StagePanel::LoadFromDirFast(const std::string& dirpath)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dirpath, files);

	Context::Instance()->tree->init(files);
}

void StagePanel::InitConnection()
{
	m_graphics.Clear();

	std::vector<ee::Sprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* from = sprites[i];
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(from))
		{
			const ecomplex::Symbol& symbol = complex->GetSymbol();
			const std::vector<s2::Sprite*>& children = symbol.GetChildren();
			for (size_t i = 0, n = children.size(); i < n; ++i)
			{
				ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
				for (size_t i = 0, n = sprites.size(); i < n; ++i)
					if (&child->GetSymbol() == &sprites[i]->GetSymbol())
						m_graphics.Connect(from, sprites[i]);
			}
		}
		else if (eanim::Sprite* anim = dynamic_cast<eanim::Sprite*>(from))
		{
			const eanim::Symbol& symbol = anim->GetSymbol();
			const std::vector<s2::AnimSymbol::Layer*>& layers = symbol.GetLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				s2::AnimSymbol::Layer* layer = layers[i];
				for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
				{
					s2::AnimSymbol::Frame* frame = layer->frames[i];
					for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
					{
						ee::Sprite* child = static_cast<ee::Sprite*>(frame->sprites[i]->GetUD());
						for (size_t i = 0, n = sprites.size(); i < n; ++i)
							if (&child->GetSymbol() == &sprites[i]->GetSymbol())
								m_graphics.Connect(from, sprites[i]);
					}
				}
			}
		}
	}
}

void StagePanel::InitPosition()
{
	std::vector<ee::Sprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	// 1 single random
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];

		Node* node = m_graphics.Query(sprite);
		if (!node || node->in.empty())
		{
			sm::vec2 pos;
			float radius = ee::Random::GetNum0To1() * 4096;
			float angle = ee::Random::GetNum(0, SM_PI*2);
			pos.x = cos(angle)*radius;
			pos.y = sin(angle)*radius;
			sprite->SetTransform(pos, 0);
		}
	}

	// 2 to
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];

		Node* node = m_graphics.Query(sprite);
//		if (node && node->in.empty())
		if (node)
		{
			float radius = 256;
			float angle = SM_PI*2 / node->out.size();
			for (size_t i = 0, n = node->out.size(); i < n; ++i)
			{
				ee::Sprite* to = node->out[i];

				sm::vec2 pos = sprite->GetPosition();
				pos.x += cos(angle*i)*radius;
				pos.y += sin(angle*i)*radius;
// 				if (to->getPosition().x != 0 || to->getPosition().y != 0)
// 					pos = (pos + to->getPosition()) * 0.5f;

			//	to->setTransform(pos, 0);
				m_graphics.Move(to, pos - to->GetPosition());
			}
		}
	}
}

}