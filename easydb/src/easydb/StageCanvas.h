#ifndef EDB_STAGE_CANVAS_H
#define EDB_STAGE_CANVAS_H

#include <drag2d.h>

namespace edb
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
		void drawConnection() const;

	private:
		d2d::SpriteBatch m_batch;

	}; // StageCanvas
}

#endif // EDB_STAGE_CANVAS_H