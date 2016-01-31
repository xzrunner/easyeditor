#ifndef RAIDEN_STAGE_CANVAS_H
#define RAIDEN_STAGE_CANVAS_H



namespace raiden
{
	class StagePanel;

	class StageCanvas : public ee::SpriteStageCanvas
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