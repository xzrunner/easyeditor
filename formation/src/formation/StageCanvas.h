#ifndef FORMATION_STAGE_CANVAS_H
#define FORMATION_STAGE_CANVAS_H



namespace formation
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

	}; // StageCanvas
}

#endif // FORMATION_STAGE_CANVAS_H