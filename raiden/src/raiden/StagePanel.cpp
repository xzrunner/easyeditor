#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"
#include "ActorInfo.h"

#include <easyshape.h>

namespace raiden
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, ee::MultiSpritesImpl(this)
{
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearSprites();
}

void StagePanel::traverseSprites(ee::IVisitor& visitor, ee::TraverseType type/* = e_allExisting*/, 
								 bool order/* = true*/) const
{
	std::vector<ee::Layer*> layers;

	ee::LayersMgrWidget* layersMgr = Context::Instance()->layers;

	switch (type)
	{
	case ee::e_editable:
		layersMgr->traverseEditableLayers(ee::FetchAllVisitor<ee::Layer>(layers));
		break;
	case ee::e_visible:
		layersMgr->traverseVisibleLayers(ee::FetchAllVisitor<ee::Layer>(layers));
		break;
	case ee::e_selectable:
		layersMgr->traverseSelectableLayers(ee::FetchAllVisitor<ee::Layer>(layers));
		break;
	case ee::e_allExisting:
		layersMgr->traverseAllLayers(ee::FetchAllVisitor<ee::Layer>(layers));
		break;
	default:
		assert(0);
		return;
	}

	bool bStop = false;
	for (size_t i = 0, n = layers.size(); i < n && !bStop; ++i)
	{
		const std::vector<ee::ISprite*>& sprites = layers[i]->getSprites();
		if (order)
		{
			std::vector<ee::ISprite*>::const_iterator itr = sprites.begin();
			for ( ; itr != sprites.end() && !bStop; ++itr)
			{
				bool hasNext = true;
				visitor.visit(*itr, hasNext);
				if (!hasNext) bStop = true;
			}
		}
		else
		{
			std::vector<ee::ISprite*>::const_reverse_iterator itr = sprites.rbegin();
			for ( ; itr != sprites.rend() && !bStop; ++itr)
			{
				bool hasNext = true;
				visitor.visit(*itr, hasNext);
				if (!hasNext) bStop = true;
			}
		}
	}
}

void StagePanel::removeSprite(ee::ISprite* sprite)
{
	ee::Layer* layer = Context::Instance()->layers->getEditedLayer();
	if (layer)
	{
		layer->remove(sprite);
		sprite->release();
	}
}

void StagePanel::insertSprite(ee::ISprite* sprite)
{
	ee::Layer* layer = Context::Instance()->layers->getEditedLayer();
	if (layer)
	{
		if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(sprite))
		{
			const std::vector<ee::ISprite*>& sprites = layer->getSprites();
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
			{
				ee::ISprite* actor = sprites[i];
				if (actor->isContain(sprite->getPosition()))
				{
					ActorInfo* info = static_cast<ActorInfo*>(actor->getUserData());
					info->symbol = &shape->getSymbol();
					info->resetOffset();
				}
			}
		}
		else
		{
			if (!sprite->getUserData())
				sprite->setUserData(new ActorInfo);

			layer->insert(sprite);
			sprite->retain();
		}
	}
}

void StagePanel::clearSprites()
{
	Context::Instance()->layers->clear();
}

void StagePanel::resetSpriteOrder(ee::ISprite* sprite, bool up)
{
	if (ee::Layer* layer = Context::Instance()->layers->getEditedLayer())
		layer->resetOrder(sprite, up);
}

void StagePanel::resetPathOffset(ee::ISymbol* symbol)
{
	std::vector<ee::ISprite*> sprites;
	traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ActorInfo* info = static_cast<ActorInfo*>(sprites[i]->getUserData());
		if (info->symbol == symbol)
			info->resetOffset();
	}
}

}