#pragma once



namespace eanim
{
	class StagePanel;

	class StageGLCanvas : public ee::OrthoCanvas
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
		class DrawVisitor : public ee::IVisitor
		{
		public:
			DrawVisitor();
			virtual void visit(ee::ICloneable* object, bool& bFetchNext);
		}; // DrawVisitor

	}; // StageGLCanvas
}

