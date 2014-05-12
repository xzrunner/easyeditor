#pragma once

#include <easyscale9.h>

namespace escale9
{
	class StagePanel;
	class ToolbarPanel;
	class ResizeCMPT;

	class ResizeOP : public escale9::ResizeBaseOP
	{
	public:
		ResizeOP(StagePanel* stage, ToolbarPanel* toolbar,
			ResizeCMPT* resizeCmpt);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onActive();

		virtual bool onDraw() const;

	private:
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;
		ResizeCMPT* m_resizeCmpt;

	}; // ResizeOP
}

