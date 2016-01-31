#ifndef _ESHAPE_STAGE_CANVAS_H_
#define _ESHAPE_STAGE_CANVAS_H_



namespace eshape
{

class StagePanel;

class StageCanvas : public ee::ShapeStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void onDraw();

private:
	void drawGuideLines();

private:
	StagePanel* m_stage;

}; // StageCanvas 

}

#endif // _ESHAPE_STAGE_CANVAS_H_