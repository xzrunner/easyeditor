#pragma once

#include <drag2d.h>

namespace escale9
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);
		virtual ~StageCanvas();

	protected:
		virtual void onDraw();

	private:
		void drawGuideLines();

	private:
		d2d::SpriteBatch m_batch;

		d2d::ShapeStyle m_bgStyle, m_nodeStyle;

	}; // StageCanvas
}

