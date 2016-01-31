#include "SpriteEditGLCanvas.h"
#include "dataset/Symbol.h"
#include "dataset/Sprite.h"
#include "tools/Render.h"

namespace eanim
{

SpriteEditGLCanvas::SpriteEditGLCanvas(ee::EditPanel* parent, Sprite* sprite)
	: ee::OrthoCanvas(parent)
	, m_sprite(sprite)
{
}

void SpriteEditGLCanvas::initGL()
{
	ee::OrthoCanvas::initGL();
	m_sprite->reloadTexture();
}

void SpriteEditGLCanvas::onDraw()
{
	Render::drawSpriteCenter(m_sprite);
	m_editPanel->drawEditTemp();
}

} // eanim