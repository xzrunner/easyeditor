#include "StagePanel.h"
#include "LibraryPanel.h"
#include "TimeLinePanel.h"
#include "StageGLCanvas.h"

#include "frame/Context.h"
#include "dataset/Layer.h"
#include "dataset/Symbol.h"
#include "dataset/Sprite.h"
#include "dataset/SpriteFactory.h"
#include "dataset/KeyFrame.h"
#include "dataset/LayersMgr.h"
#include "operator/ArrangeSpriteOP.h"

namespace eanim
{

StagePanel::StagePanel(wxWindow* parent)
	: EditPanel(parent), MultiSpritesImpl(parent)
{
	SetDropTarget(new DragSymbolTarget(this));
	m_editOP = new ArrangeSpriteOP(this);
	m_canvas = new StageGLCanvas(this);
	SetFocus();
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearSprites();
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, 
								 d2d::TraverseType type/* = d2d::e_allExisting*/,
								 bool order/* = true*/) const
{
	std::vector<Sprite*>::const_iterator itr = m_sprites.begin();
	for ( ; itr != m_sprites.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] == sprite)
		{
			Context::Instance()->timeline->getLayerMgr()->removeSprite(m_sprites[i]);
			m_sprites[i]->release();
			m_sprites.erase(m_sprites.begin() + i);
			break;
		}
	}
}

void StagePanel::clearSprites()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
	m_sprites.clear();
	m_spriteSelection->clear();
}

void StagePanel::insertSymbol(int index, wxCoord x, wxCoord y)
{
	Context* context = Context::Instance();

	Symbol* symbol = context->library->getSymbol(index);
	if (!symbol) return;
	Layer* layer = context->timeline->getCurrLayer();
	if (!layer) return;

	d2d::Vector pos = transPosScreenToProject(x, y);
	Sprite* sprite = SpriteFactory::Instance()->createMorphingSprite(symbol, pos, true);
	bool success = layer->insertSprite(sprite, context->timeline->getCurrFrame());
	if (!success) delete sprite;
	else loadCurrFrameSprites();
}

void StagePanel::insertSkeleton(int index, wxCoord x, wxCoord y)
{
	Context* context = Context::Instance();

	WholeSkeleton* skeleton = context->library->getSkeleton(index);
	if (!skeleton) return;
	Layer* layer = context->timeline->getCurrLayer();
	if (!layer) return;

	Sprite* sprite = SpriteFactory::Instance()->createSkeletalSprite(skeleton);
	bool success = layer->insertSprite(sprite, context->timeline->getCurrFrame());
	if (!success) delete sprite;
	else loadCurrFrameSprites();
}

void StagePanel::loadCurrFrameSprites()
{
	TimeLinePanel* timeline = Context::Instance()->timeline;

	m_sprites.clear();
	LayersMgr* layersMgr = timeline->getLayerMgr();
	for (size_t i = 0, n = layersMgr->size(); i < n; ++i)
	{
		Layer* layer = layersMgr->getLayer(i);
		KeyFrame* frame = layer->getCurrKeyFrame(timeline->getCurrFrame());
		if (frame) 
		{
			const std::vector<Sprite*>& sprites = frame->getAllSprites();
			copy(sprites.begin(), sprites.end(), back_inserter(m_sprites));
		}
	}
	Refresh();
	timeline->Refresh();
}

void StagePanel::resetCanvas()
{
	m_canvas->resetInitState();
	Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::DragSymbolTarget::DragSymbolTarget(StagePanel* panel)
{
	m_panel = panel;
}

bool StagePanel::DragSymbolTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	if (sType == LibraryPanel::SYMBOL_LIST_NAME)
		m_panel->insertSymbol(index, x, y);
	else if (sType == LibraryPanel::SKELETON_LIST_NAME)
		m_panel->insertSkeleton(index, x, y);

	return true;
}

} // eanim