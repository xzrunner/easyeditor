#ifndef _EASYMODELING_SELECT_BODY_OP_H_
#define _EASYMODELING_SELECT_BODY_OP_H_

#include <ee/SelectSpritesOP.h>
#include <ee/Visitor.h>

namespace emodeling
{

class Body;

class SelectBodyOP : public ee::SelectSpritesOP
{
public:
	SelectBodyOP(wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, 
		ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	class DrawSelectedVisitor : public ee::Visitor<ee::Sprite>
	{
	public:
		virtual void Visit(const ee::SprPtr& spr, bool& next);
	}; // DrawSelectedVisitor

private:
	Body* m_mouseOn;

}; // SelectBodyOP

}

#endif // _EASYMODELING_SELECT_BODY_OP_H_
