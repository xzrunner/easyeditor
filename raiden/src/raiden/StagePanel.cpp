#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"
#include "ActorInfo.h"

#include <easyshape.h>

namespace raiden
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(this)
{
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearSprites();
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, d2d::TraverseType type/* = e_allExisting*/, 
								 bool order/* = true*/) const
{
	std::vector<d2d::Layer*> layers;

	d2d::LayersMgrWidget* layersMgr = Context::Instance()->layers;

	switch (type)
	{
	case d2d::e_editable:
		layersMgr->traverseEditableLayers(d2d::FetchAllVisitor<d2d::Layer>(layers));
		break;
	case d2d::e_visible:
		layersMgr->traverseVisibleLayers(d2d::FetchAllVisitor<d2d::Layer>(layers));
		break;
	case d2d::e_selectable:
		layersMgr->traverseSelectableLayers(d2d::FetchAllVisitor<d2d::Layer>(layers));
		break;
	case d2d::e_allExisting:
		layersMgr->traverseAllLayers(d2d::FetchAllVisitor<d2d::Layer>(layers));
		break;
	default:
		assert(0);
		return;
	}

	bool bStop = false;
	for (size_t i = 0, n = layers.size(); i < n && !bStop; ++i)
	{
		const std::vector<d2d::ISprite*>& sprites = layers[i]->getSprites();
		if (order)
		{
			std::vector<d2d::ISprite*>::const_iterator itr = sprites.begin();
			for ( ; itr != sprites.end() && !bStop; ++itr)
			{
				bool hasNext = true;
				visitor.visit(*itr, hasNext);
				if (!hasNext) bStop = true;
			}
		}
		else
		{
			std::vector<d2d::ISprite*>::const_reverse_iterator itr = sprites.rbegin();
			for ( ; itr != sprites.rend() && !bStop; ++itr)
			{
				bool hasNext = true;
				visitor.visit(*itr, hasNext);
				if (!hasNext) bStop = true;
			}
		}
	}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::Layer* layer = Context::Instance()->layers->getEditedLayer();
	if (layer)
	{
		layer->remove(sprite);
		sprite->release();
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::Layer* layer = Context::Instance()->layers->getEditedLayer();
	if (layer)
	{
		if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(sprite))
		{
			const std::vector<d2d::ISprite*>& sprites = layer->getSprites();
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
			{
				d2d::ISprite* actor = sprites[i];
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

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	if (d2d::Layer* layer = Context::Instance()->layers->getEditedLayer())
		layer->resetOrder(sprite, up);
}

void StagePanel::resetPathOffset(d2d::ISymbol* symbol)
{
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ActorInfo* info = static_cast<ActorInfo*>(sprites[i]->getUserData());
		if (info->symbol == symbol)
			info->resetOffset();
	}
}

}