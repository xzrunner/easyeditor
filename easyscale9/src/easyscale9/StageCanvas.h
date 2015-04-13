#pragma once

#include <drag2d.h>

namespace escale9
{
	class StagePanel;
	class ToolbarPanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);
		virtual ~StageCanvas();

		void setToolbarPanel(ToolbarPanel* toolbar) {
			m_toolbar = toolbar;
		}

	protected:
		virtual void onDraw();

	private:
		void drawGuideLines();

	private:
		d2d::ShapeStyle m_bgStyle, m_nodeStyle;

		ToolbarPanel* m_toolbar;

	}; // StageCanvas
}

