#ifndef EPACKER_STAGE_CANVAS_H
#define EPACKER_STAGE_CANVAS_H

#include <drag2d.h>

namespace epacker
{
	class StagePanel;

	class StageCanvas : public d2d::SpriteStageCanvas
	{
	public:
		StageCanvas(StagePanel* parent);

	protected:
		virtual void onDraw();

	private:
		void drawGuideLines();

		void drawCross();
		void drawRect();

	}; // StageCanvas
}

#endif // EPACKER_STAGE_CANVAS_H