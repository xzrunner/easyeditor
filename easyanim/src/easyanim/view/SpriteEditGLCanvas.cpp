#include "SpriteEditGLCanvas.h"
#include "dataset/Symbol.h"
#include "dataset/Sprite.h"
#include "tools/Render.h"

namespace eanim
{

SpriteEditGLCanvas::SpriteEditGLCanvas(d2d::EditPanel* parent, Sprite* sprite)
	: d2d::OrthoCanvas(parent)
	, m_sprite(sprite)
{
}

void SpriteEditGLCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_sprite->reloadTexture();
}

void SpriteEditGLCanvas::onDraw()
{
	Render::drawSpriteCenter(m_sprite);
	m_editPanel->drawEditTemp();
}

} // eanim