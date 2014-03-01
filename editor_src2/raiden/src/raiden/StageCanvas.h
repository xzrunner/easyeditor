#ifndef RAIDEN_STAGE_CANVAS_H
#define RAIDEN_STAGE_CANVAS_H

#include <drag2d.h>

namespace raiden
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

#endif // RAIDEN_STAGE_CANVAS_H