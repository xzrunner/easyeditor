#ifndef FORMATION_PREVIEW_CANVAS_H
#define FORMATION_PREVIEW_CANVAS_H



namespace formation
{
	class PreviewCanvas : public ee::OrthoCanvas
	{
	public:
		PreviewCanvas(ee::EditPanel* editPanel);
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
		class DrawVisitor : public ee::IVisitor
		{
		public:
			DrawVisitor(float yOffset, float scale) 
				: m_yOffset(yOffset), m_scale(scale) {}

			virtual void visit(ee::Object* object, bool& bFetchNext);

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