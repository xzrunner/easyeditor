#ifndef FORMATION_PREVIEW_CANVAS_H
#define FORMATION_PREVIEW_CANVAS_H

#include <drag2d.h>

namespace formation
{
	class PreviewCanvas : public d2d::OrthoCanvas
	{
	public:
		PreviewCanvas(d2d::EditPanel* editPanel);
		virtual ~PreviewCanvas();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		enum
		{
			TIMER_ID = 1000
		};

		static const int FRAME_RATE = 24;

	private:
		class DrawVisitor : public d2d::IVisitor
		{
		public:
			DrawVisitor(float yOffset, float scale) 
				: m_yOffset(yOffset), m_scale(scale) {}

			virtual void visit(d2d::Object* object, bool& bFetchNext);

		private:
			float m_yOffset;
			float m_scale;

		}; // DrawVisitor

	private:
		wxTimer m_timer;
		float m_yOffset;

		float m_offset;
		float m_scale;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

#endif // FORMATION_PREVIEW_CANVAS_H