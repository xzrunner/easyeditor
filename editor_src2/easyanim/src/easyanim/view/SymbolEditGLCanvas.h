#pragma once
#include <drag2d.h>

namespace eanim
{
	class Symbol;

	class SymbolEditGLCanvas : public d2d::OrthoCanvas
	{
	public:
		enum e_DrawMode
		{
			DrawInTexture,
			DrawInPixels
		};

		struct RenderSetting
		{
			bool drawPixelBound;
			bool drawOriginalSymbol;

			RenderSetting()
			{
				drawPixelBound = true;
				drawOriginalSymbol = true;
			}
		};

	public:
		SymbolEditGLCanvas(d2d::EditPanel* parent, Symbol* symbol);

		void setDrawMode(e_DrawMode mode);

		d2d::RawPixels* getRawPixels();
		d2d::RawPixels::PixelBuf& getSelectedPixels();

		RenderSetting& getRenderSetting();

	protected:
		virtual void initGL();
		virtual void onDraw();

	private:
		Symbol* m_symbol;

		e_DrawMode m_drawMode;

		d2d::RawPixels::PixelBuf m_selectedPixels;

		RenderSetting m_renderSetting;

	}; // SymbolEditGLCanvas
}

