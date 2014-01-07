#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteOP.h"

#include "frame/Context.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"

namespace eanim
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, MultiSpritesImpl(parent)
{
//	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this);
	m_editOP = new ArrangeSpriteOP(this);
	m_canvas = new StageCanvas(this);
	SetDropTarget(new DragSymbolTarget());
}

StagePanel::~StagePanel()
{
	clear();
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
	Context* context = Context::Instance();

	if (context->currLayer == -1 || context->currFrame == -1)
		return;

	const std::vector<Layer*>& layers = context->layers.getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		KeyFrame* frame = layers[i]->getCurrKeyFrame(context->currFrame);
		if (!frame) continue;

		if (order)
		{
			for (int i = 0, n = frame->size(); i < n; ++i) {
				bool hasNext;
				visitor.visit(const_cast<d2d::ISprite*>(frame->getSprite(i)), hasNext);
				if (!hasNext) break;
			}
		}
		else
		{
			for (int i = frame->size() - 1; i >= 0; --i) {
				bool hasNext;
				visitor.visit(const_cast<d2d::ISprite*>(frame->getSprite(i)), hasNext);
				if (!hasNext) break;
			}
		}
	}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	Context* context = Context::Instance();
	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);

	bool success = frame->remove(sprite);
	if (success)
		context->keysPanel->Refresh();
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	Context* context = Context::Instance();
	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
	frame->insert(sprite);
	Refresh();
	context->keysPanel->Refresh();
}

void StagePanel::clearSprites()
{
	Context* context = Context::Instance();
	context->layers.clear();
	context->currLayer = -1;
	context->currFrame = -1;

// 	Context* context = Context::Instance();
// 	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
// 	frame->clear();
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	Context* context = Context::Instance();
	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
	frame->reorder(sprite, up);
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

bool StagePanel::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	StagePanel* stage = static_cast<StagePanel*>(Context::Instance()->stage);
	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(Context::Instance()->library);

	d2d::Vector pos = stage->transPosScreenToProject(x, y);

	d2d::ISymbol* symbol = library->getSymbol(index);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	sprite->translate(pos);

	stage->insertSprite(sprite);

	return true;
}
} // eanim