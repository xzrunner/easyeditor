#pragma once

#include <drag2d.h>

namespace e9patch
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

	}; // StageCanvas
}

