
#ifndef ESHAPE_STAGE_CANVAS_H
#define ESHAPE_STAGE_CANVAS_H

#include <drag2d.h>

namespace eshape
{
	class StagePanel;

	class StageCanvas : public d2d::ShapeStageCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);

	protected:
		virtual void onDraw();

	private:
		void drawGuideLines();

	}; // StageCanvas 
}

#endif // ESHAPE_STAGE_CANVAS_H