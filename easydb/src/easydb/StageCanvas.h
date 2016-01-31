#ifndef EDB_STAGE_CANVAS_H
#define EDB_STAGE_CANVAS_H



namespace edb
{
	class StagePanel;

	class StageCanvas : public ee::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);
		virtual ~StageCanvas();

	protected:
		virtual void OnDrawSprites() const;

	private:
		void drawConnection() const;

	private:
		StagePanel* m_stage_panel;

		ee::SpriteBatch m_batch;

	}; // StageCanvas
}

#endif // EDB_STAGE_CANVAS_H