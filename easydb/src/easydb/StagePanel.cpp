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
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->RemoveReference();
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
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
			sym->RemoveReference();
			ee::InsertSpriteSJ::Instance()->Insert(spr);
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

	std::vector<ee::Sprite*> sprs;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* from = sprs[i];
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(from))
		{
			const s2::ComplexSymbol* sym = dynamic_cast<const s2::ComplexSymbol*>(complex->GetSymbol());
			const std::vector<s2::Sprite*>& children = sym->GetChildren();
			for (size_t i = 0, n = children.size(); i < n; ++i)
			{
				ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
				for (size_t i = 0, n = sprs.size(); i < n; ++i)
					if (child->GetSymbol() == sprs[i]->GetSymbol())
						m_graphics.Connect(from, sprs[i]);
			}
		}
		else if (eanim::Sprite* anim = dynamic_cast<eanim::Sprite*>(from))
		{
			const s2::AnimSymbol* sym = dynamic_cast<const s2::AnimSymbol*>(anim->GetSymbol());
			const std::vector<s2::AnimSymbol::Layer*>& layers = sym->GetLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				s2::AnimSymbol::Layer* layer = layers[i];
				for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
				{
					s2::AnimSymbol::Frame* frame = layer->frames[i];
					for (size_t i = 0, n = frame->sprs.size(); i < n; ++i)
					{
						ee::Sprite* child = dynamic_cast<ee::Sprite*>(frame->sprs[i]);
						for (size_t i = 0, n = sprs.size(); i < n; ++i)
							if (child->GetSymbol() == sprs[i]->GetSymbol())
								m_graphics.Connect(from, sprs[i]);
					}
				}
			}
		}
	}
}

void StagePanel::InitPosition()
{
	std::vector<ee::Sprite*> sprs;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));

	// 1 single random
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];

		Node* node = m_graphics.Query(spr);
		if (!node || node->in.empty())
		{
			sm::vec2 pos;
			float radius = ee::Random::GetNum0To1() * 4096;
			float angle = ee::Random::GetNum(0, SM_PI*2);
			pos.x = cos(angle)*radius;
			pos.y = sin(angle)*radius;
			spr->SetPosition(pos);
			spr->SetAngle(0);
		}
	}

	// 2 to
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];

		Node* node = m_graphics.Query(spr);
//		if (node && node->in.empty())
		if (node)
		{
			float radius = 256;
			float angle = SM_PI*2 / node->out.size();
			for (size_t i = 0, n = node->out.size(); i < n; ++i)
			{
				ee::Sprite* to = node->out[i];

				sm::vec2 pos = spr->GetPosition();
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