
#ifndef LIBSHAPE_STAGE_CANVAS_H
#define LIBSHAPE_STAGE_CANVAS_H

#include <drag2d.h>

namespace libshape
{
	class StagePanel;

	class StageCanvas : public d2d::ShapeStageCanvas
	{
	public:
		StageCanvas(StagePanel* stage);

	public:
		static void drawGuideLines();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);
		void onKeyDown(wxKeyEvent& event);

	private:
		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}

#endif // LIBSHAPE_STAGE_CANVAS_H