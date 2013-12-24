#include "SymbolEditGLCanvas.h"
#include "dataset/Symbol.h"
#include "dataset/MorphingSprite.h"
#include "tools/Render.h"

namespace eanim
{

SymbolEditGLCanvas::SymbolEditGLCanvas(d2d::EditPanel* parent, Symbol* symbol)
	: d2d::OrthoCanvas(parent)
	, m_symbol(symbol)
	, m_drawMode(DrawInPixels)
{
}

void SymbolEditGLCanvas::setDrawMode(e_DrawMode mode)
{
	m_drawMode = mode;
}

d2d::RawPixels* SymbolEditGLCanvas::getRawPixels()
{
	return m_symbol->getRawPixels();
}

d2d::RawPixels::PixelBuf& SymbolEditGLCanvas::getSelectedPixels()
{
	return m_selectedPixels;
}

SymbolEditGLCanvas::RenderSetting& SymbolEditGLCanvas::getRenderSetting()
{
	return m_renderSetting;
}

void SymbolEditGLCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_symbol->reloadTexture();
}

void SymbolEditGLCanvas::onDraw()
{
	switch (m_drawMode)
	{
	case DrawInTexture:
		{
			MorphingSprite sprite(m_symbol, d2d::Vector(0, 0), false);
			d2d::SpriteDraw::drawSprite(&sprite);
		}
		break;
	case DrawInPixels:
		{
			glBindTexture(GL_TEXTURE_2D, NULL);
			if (m_renderSetting.drawOriginalSymbol)
				Render::drawRawPixels(*m_symbol->getRawPixels());
			if (m_renderSetting.drawOriginalSymbol)
				Render::drawRawPixelsSelectedFlag(*m_symbol->getRawPixels(), m_selectedPixels);	
			else
				Render::drawRawPixelsSelected(*m_symbol->getRawPixels(), m_selectedPixels);
			if (m_renderSetting.drawPixelBound)
				Render::drawRawPixelsBound(*m_symbol->getRawPixels());
		}
		break;
	}
	m_editPanel->drawEditTemp();
}

} // eanim