
#include "DesignerPage.h"

#include "dataset/Actor.h"
#include "dataset/Layer.h"
#include "dataset/Scene.h"
#include "dataset/Game.h"
#include "view/Context.h"
#include "view/PropertySettingPanel.h"
#include "view/LibraryPanel.h"
#include "view/ActorPropertySetting.h"
#include "view/ScenesPage.h"
#include "view/SceneItem.h"
#include "view/LayerItem.h"
#include "view/ActorsList.h"
#include "view/ActorItem.h"
#include "view/StagePanel.h"
#include "view/BackstagePanel.h"

using namespace ebuilder;

DesignerPage::DesignerPage(wxWindow* parent,
						   wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame)
	, ee::MultiSpritesImpl(this)
{
	m_editOP = new ArrangeActorOP(this, NULL);
	m_canvas = new Canvas(this);

	SetDropTarget(new DragActorTarget(this));
}

void DesignerPage::traverseSprites(ee::IVisitor& visitor, 
								   ee::TraverseType type/* = e_allExisting*/, 
								   bool order/* = true*/) const
{
	SceneItem* scene = dynamic_cast<SceneItem*>(Context::Instance()->library->getSceneList()->getItem());

	if (!scene) return;

	const std::vector<Layer*>& layers = scene->getScene()->getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		Layer* layer = layers[i];

		if ((type == ee::e_editable && !layer->isEditable) ||
			(type == ee::e_visible && !layer->isVisible))
			continue;

		const std::vector<Actor*>& actors = layer->getActors();
		if (order)
		{
			std::vector<Actor*>::const_iterator itr = actors.begin();
			for ( ; itr != actors.end(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
		else
		{
			std::vector<Actor*>::const_reverse_iterator itr = actors.rbegin();
			for ( ; itr != actors.rend(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}

		const std::vector<ee::TextSprite*>& texts = layer->getTexts();
		for (size_t i = 0, n = texts.size(); i < n; ++i)
		{
			bool hasNext;
			visitor.visit(texts[i], hasNext);
			if (!hasNext) break;
		}
	}
}

void DesignerPage::removeSprite(ee::ISprite* sprite)
{
	LayerItem* layer = dynamic_cast<LayerItem*>(Context::Instance()->library->getLayerList()->getItem());
	if (!layer) return;

	layer->getLayer()->remove(sprite);
	layer->refresh();
	updateCodePage();
}

void DesignerPage::insertSprite(ee::ISprite* sprite)
{
	LayerItem* layer = dynamic_cast<LayerItem*>(Context::Instance()->library->getLayerList()->getItem());
	if (!layer) return;

	if (Actor* actor = dynamic_cast<Actor*>(sprite))
	{
		actor->retain();
		layer->getLayer()->insert(actor);
		refresh();
		updateCodePage();

		Context::Instance()->library->getLayerList()->getItem()->refresh();
		Context::Instance()->library->getSceneList()->getItem()->refresh();
	}
	else if (ee::TextSprite* text = dynamic_cast<ee::TextSprite*>(sprite))
	{
		text->retain();
		layer->getLayer()->insert(text);
		refresh();
		updateCodePage();
	}
}

void DesignerPage::clearSprites()
{
}

void DesignerPage::resetSpriteOrder(ee::ISprite* sprite, bool up)
{
	LayerItem* layer = dynamic_cast<LayerItem*>(Context::Instance()->library->getLayerList()->getItem());
	if (!layer) return;

	layer->getLayer()->resetOrder(sprite, up);
}

void DesignerPage::updateCodePage()
{
	SceneItem* scene = dynamic_cast<SceneItem*>(Context::Instance()->library->getSceneList()->getItem());
	if (scene)
		Context::Instance()->stage->updateCodePages(scene);
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::Canvas
//////////////////////////////////////////////////////////////////////////

DesignerPage::Canvas::Canvas(DesignerPage* panel)
	: ee::SpriteStageCanvas(panel, panel)
{
	setBgColor(ee::Colorf(0.8f, 0.8f, 0.8f));
}

void DesignerPage::Canvas::onDraw()
{
	drawBackground();	

	ee::SpriteStageCanvas::onDraw();

	drawCenter();
}

void DesignerPage::Canvas::drawBackground()
{
	ee::PrimitiveDraw::rect(ee::Vector(), 
		Game::WIDTH * 0.5f, 
		Game::HEIGHT * 0.5f, 
		ee::BLACK_RECT_FACE);
}

void DesignerPage::Canvas::drawCenter()
{
	const int hEdge = 15;
	ee::PrimitiveDraw::drawLine(ee::Vector(-hEdge, 0), ee::Vector(hEdge, 0), 
		ee::Colorf(1, 0, 0), 2);
	ee::PrimitiveDraw::drawLine(ee::Vector(0, -hEdge), ee::Vector(0, hEdge), 
		ee::Colorf(1, 0, 0), 2);
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::DragActorTarget
//////////////////////////////////////////////////////////////////////////

DesignerPage::DragActorTarget::
DragActorTarget(DesignerPage* editPanel)
	: m_editPanel(editPanel)
{
}

bool DesignerPage::DragActorTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

 	if (ee::ImageSymbol* symbol = dynamic_cast<ee::ImageSymbol*>
		(Context::Instance()->library->getSymbol(index)))
 	{
 		ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);

		Actor* actor = new Actor(symbol);
 		actor->translate(pos);
 		m_editPanel->insertSprite(actor);
		static_cast<ActorsList*>(Context::Instance()->library->getActorList())->insert(new ActorItem(actor));
 		actor->release();
 	}
	else
	{
		ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		ee::ISprite* sprite = ee::SpriteFactory::Instance()->create(
			Context::Instance()->library->getSymbol(index));
		sprite->translate(pos);
		m_editPanel->insertSprite(sprite);
		sprite->release();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::SelectActorOP
//////////////////////////////////////////////////////////////////////////

DesignerPage::SelectActorOP::
SelectActorOP(ee::EditPanel* editPanel, 
			  ee::MultiSpritesImpl* spritesImpl,
			  ee::PropertySettingPanel* propertyPanel,
			  ee::AbstractEditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

DesignerPage::SelectActorOP::
~SelectActorOP()
{
	clearClipboard();
}

bool DesignerPage::SelectActorOP::
onKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::onKeyDown(keyCode))
		return true;

	if (wxGetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		clearClipboard();

		std::vector<Actor*> actors;
		m_selection->traverse(ee::FetchAllVisitor<Actor>(actors));
		for (size_t i = 0, n = actors.size(); i < n; ++i)
			m_clipboard.push_back(actors[i]->clone());
	}
	else if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
	{
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		{
			Actor* actor = m_clipboard[i]->clone();
			m_spritesImpl->insertSprite(actor);
			actor->release();
		}
	}

	return false;
}

bool DesignerPage::SelectActorOP::
onMouseLeftUp(int x, int y)
{
	if (ee::SelectSpritesOP::onMouseLeftUp(x, y))
		return true;

	if (m_selection->size() != 1)
		clearBackstage();

	return false;
}

bool DesignerPage::SelectActorOP::
clear()
{
	if (ee::SelectSpritesOP::clear())
		return true;

	clearBackstage();
//	clearClipboard();

	return false;
}

ee::IPropertySetting* DesignerPage::SelectActorOP::
createPropertySetting(ee::ISprite* sprite) const
{
	if (ee::TextSprite* text = dynamic_cast<ee::TextSprite*>(sprite))
	{
		return new ee::TextPropertySetting(m_editPanel, text);
	}
	else if (sprite)
	{
		Context* context = Context::Instance();

		context->backstage->initFromActor(static_cast<Actor*>(sprite));

		return new ActorPropertySetting(static_cast<Actor*>(sprite), 
			context->library->getActorList());
	}
	else
	{
		return NULL;
	}
}

void DesignerPage::SelectActorOP::
clearBackstage()
{
	Context::Instance()->backstage->initFromActor(NULL);
}

void DesignerPage::SelectActorOP::
clearClipboard()
{
	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		m_clipboard[i]->release();
	m_clipboard.clear();
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::TranslateActorState
//////////////////////////////////////////////////////////////////////////

void DesignerPage::TranslateActorState::
Translate(const ee::Vector& offset) 
{
	ee::TranslateSpriteState::Translate(offset);

	LibraryPanel* library = Context::Instance()->library;
	library->getScenePage()->refreshThumbnail();
	library->getScenePage()->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::RotateActorState
//////////////////////////////////////////////////////////////////////////

void DesignerPage::RotateActorState::
Rotate(const ee::Vector& dst) 
{
	ee::RotateSpriteState::Rotate(dst);

	LibraryPanel* library = Context::Instance()->library;
	library->getScenePage()->refreshThumbnail();
	library->getScenePage()->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::ScaleActorState
//////////////////////////////////////////////////////////////////////////

void DesignerPage::ScaleActorState::
Scale(const ee::Vector& curr) 
{
	ee::ScaleSpriteState::Scale(curr);

	LibraryPanel* library = Context::Instance()->library;
	library->getScenePage()->refreshThumbnail();
	library->getScenePage()->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::ArrangeActorImpl
//////////////////////////////////////////////////////////////////////////

DesignerPage::ArrangeActorImpl::
ArrangeActorImpl(DesignerPage* editPanel)
	: ee::ArrangeSpriteImpl(editPanel, editPanel, Context::Instance()->property)
	, m_editPanel(editPanel)
{
}

void DesignerPage::ArrangeActorImpl::
onMouseLeftUp(int x, int y)
{
	ee::ArrangeSpriteImpl::onMouseLeftUp(x, y);
	static_cast<DesignerPage*>(m_editPanel)->updateCodePage();
}

void DesignerPage::ArrangeActorImpl::
onMouseRightUp(int x, int y)
{
	ee::ArrangeSpriteImpl::onMouseRightUp(x, y);
	static_cast<DesignerPage*>(m_editPanel)->updateCodePage();
}

ee::IArrangeSpriteState* DesignerPage::ArrangeActorImpl::
CreateTransalteState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const
{
	return new TranslateActorState(selection, first_pos);
}

ee::IArrangeSpriteState* DesignerPage::ArrangeActorImpl::
CreateRotateState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const
{
	return new RotateActorState(selection, first_pos);
}

ee::IArrangeSpriteState* DesignerPage::
ArrangeActorImpl::CreateScaleState(ee::ISprite* sprite, const ee::SpriteCtrlNode::Node& ctrl_node) const
{
	return new ScaleActorState(sprite, ctrl_node);
}

//////////////////////////////////////////////////////////////////////////
// class DesignerPage::ArrangeActorOP
//////////////////////////////////////////////////////////////////////////

DesignerPage::ArrangeActorOP::
ArrangeActorOP(DesignerPage* editPanel, ee::AbstractEditCMPT* callback)
	: ee::ArrangeSpriteOP<SelectActorOP>(editPanel, editPanel, Context::Instance()->property, 
	callback, false, true, new ArrangeActorImpl(editPanel))
{
}
