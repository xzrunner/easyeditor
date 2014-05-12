#pragma once

#include <drag2d.h>

namespace eanim
{
	class StagePanel;

	class StageGLCanvas : public d2d::OrthoCanvas
	{
	public:
		StageGLCanvas(StagePanel* editPanel);

	protected:
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);
		void onKeyDown(wxKeyEvent& event);

	private:
		void drawbackground() const;

	private:
		class DrawVisitor : public d2d::IVisitor
		{
		public:
			DrawVisitor();
			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);
		}; // DrawVisitor

	}; // StageGLCanvas
}

