#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"
#include "ActorInfo.h"

using namespace formation;

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(this, Context::Instance()->library)
{
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
	EditPanel::clear();
	SpritesPanelImpl::clearSprites();
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);

	if (!sprite->getUserData())
		sprite->setUserData(new ActorInfo);
}

void StagePanel::resetAllSprites(float sx, float sy)
{
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		float x = sprite->getPosition().x * sx,
			y = sprite->getPosition().y * sy;
		sprite->setTransform(d2d::Vector(x, y), sprite->getAngle());
	}
}