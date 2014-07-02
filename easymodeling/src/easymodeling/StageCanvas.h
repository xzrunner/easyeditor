
#ifndef EMODELING_STAGE_CANVAS_H
#define EMODELING_STAGE_CANVAS_H

#include <drag2d.h>

namespace emodeling
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
		void drawSprites();

		void drawCrossLine() const;
		void drawLines() const;

	private:
		class DrawJointVisitor : public d2d::IVisitor
		{
		public:
			DrawJointVisitor(const d2d::Screen& scr) : m_scr(scr) {}
			virtual void visit(d2d::Object* object, bool& bFetchNext);

		private:
			const d2d::Screen& m_scr;
		};

	}; // StageCanvas
}

#endif // EMODELING_STAGE_CANVAS_H
