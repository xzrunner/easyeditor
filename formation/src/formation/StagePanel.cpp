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

void StagePanel::insertSprite(ee::ISprite* sprite)
{
	ee::SpritesPanelImpl::insertSprite(sprite);

	if (!sprite->getUserData())
		sprite->setUserData(new ActorInfo);
}

void StagePanel::resetAllSprites(float sx, float sy)
{
	std::vector<ee::ISprite*> sprites;
	traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::ISprite* sprite = sprites[i];
		float x = sprite->getPosition().x * sx,
			y = sprite->getPosition().y * sy;
		sprite->setTransform(ee::Vector(x, y), sprite->getAngle());
	}
}