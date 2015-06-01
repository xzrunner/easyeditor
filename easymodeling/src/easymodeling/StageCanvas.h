#ifndef _EASYMODELING_STAGE_CANVAS_H_
#define _EASYMODELING_STAGE_CANVAS_H_

#include <drag2d.h>

namespace emodeling
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* editPanel);
	virtual ~StageCanvas();

protected:
	virtual void onDraw();

private:
	void drawGuideLines();
	void drawSprites();

	void drawCrossLine() const;
	void drawLines() const;

private:
	class DrawJointVisitor : public d2d::IVisitor
	{
	public:
		virtual void visit(d2d::Object* object, bool& bFetchNext);
	};

}; // StageCanvas

}

#endif // _EASYMODELING_STAGE_CANVAS_H_
