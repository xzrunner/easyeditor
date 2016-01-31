#include "SymbolEditGLCanvas.h"
#include "dataset/Symbol.h"
#include "dataset/MorphingSprite.h"
#include "tools/Render.h"

namespace eanim
{

SymbolEditGLCanvas::SymbolEditGLCanvas(ee::EditPanel* parent, Symbol* symbol)
	: ee::OrthoCanvas(parent)
	, m_symbol(symbol)
	, m_drawMode(DrawInPixels)
{
}

void SymbolEditGLCanvas::setDrawMode(e_DrawMode mode)
{
	m_drawMode = mode;
}

ee::RawPixels* SymbolEditGLCanvas::getRawPixels()
{
	return m_symbol->getRawPixels();
}

ee::RawPixels::PixelBuf& SymbolEditGLCanvas::getSelectedPixels()
{
	return m_selectedPixels;
}

SymbolEditGLCanvas::RenderSetting& SymbolEditGLCanvas::getRenderSetting()
{
	return m_renderSetting;
}

void SymbolEditGLCanvas::initGL()
{
	ee::OrthoCanvas::initGL();
	m_symbol->reloadTexture();
}

void SymbolEditGLCanvas::onDraw()
{
	switch (m_drawMode)
	{
	case DrawInTexture:
		{
			MorphingSprite sprite(m_symbol, ee::Vector(0, 0), false);
			ee::SpriteDraw::drawSprite(&sprite);
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