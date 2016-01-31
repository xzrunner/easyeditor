#pragma once


namespace eanim
{
	class Symbol;

	class SymbolEditGLCanvas : public ee::OrthoCanvas
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
		SymbolEditGLCanvas(ee::EditPanel* parent, Symbol* symbol);

		void setDrawMode(e_DrawMode mode);

		ee::RawPixels* getRawPixels();
		ee::RawPixels::PixelBuf& getSelectedPixels();

		RenderSetting& getRenderSetting();

	protected:
		virtual void initGL();
		virtual void onDraw();

	private:
		Symbol* m_symbol;

		e_DrawMode m_drawMode;

		ee::RawPixels::PixelBuf m_selectedPixels;

		RenderSetting m_renderSetting;

	}; // SymbolEditGLCanvas
}

