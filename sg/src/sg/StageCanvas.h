#ifndef _SG_STAGE_CANVAS_H_
#define _SG_STAGE_CANVAS_H_

#include <ee/OrthoCanvas.h>

namespace ee { class Symbol; class Sprite; }

namespace sg
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

	void SetBackground(ee::Symbol* background);

	static ee::Vector TransToBirdView(const ee::Vector& pos);
	static ee::Vector TransToFlatView(const ee::Vector& pos);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawBackground() const;
	void DrawGuideLines() const;
	void DrawGrass() const;
	void DrawGrids() const;
	void DrawSprites() const;
	void DrawArrow() const;
	void DrawAttackRegion() const;

private:
	StagePanel* m_stage;

	ee::Sprite* m_background;

}; // StageCanvas

}

#endif // _SG_STAGE_CANVAS_H_