#pragma once



namespace coceditor
{
	class StagePanel;

	class StageCanvas : public ee::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);
		virtual ~StageCanvas();

	protected:
		virtual void onDraw();

	private:
		void drawGuideLines();

	private:
		ee::SpriteBatch m_batch;

	}; // StageCanvas
}

