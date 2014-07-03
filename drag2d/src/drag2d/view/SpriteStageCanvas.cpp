#include "SpriteStageCanvas.h"

#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "render/DrawSpritesVisitor.h"

namespace d2d
{

SpriteStageCanvas::SpriteStageCanvas(EditPanel* editPanel, MultiSpritesImpl* spritesImpl)
	: OrthoCanvas(editPanel)
	, m_spritesImpl(spritesImpl)
	, m_batch(500, SpriteBatch::USAGE_STATIC)
{
}

void SpriteStageCanvas::onDraw()
{
	m_spritesImpl->traverseSprites(DrawSpritesVisitor(m_screen, m_batch), e_visible);
	m_editPanel->drawEditTemp(m_screen);
}

} // d2d