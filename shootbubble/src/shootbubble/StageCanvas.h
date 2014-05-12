#ifndef SHOOTBUBBLE_STAGE_CANVAS_H_
#define SHOOTBUBBLE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace shootbubble
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

	private:
		StagePanel* m_stagePanel;

	}; // StageCanvas
}

#endif // SHOOTBUBBLE_STAGE_CANVAS_H_