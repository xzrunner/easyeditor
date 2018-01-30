#pragma once

#include "ee2/CamControlOP.h"

#include <SM_Vector.h>

namespace ee2
{

class DrawSelectRectOP : public CamControlOP
{
public:
	DrawSelectRectOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		s2::Camera& cam, ee0::SubjectMgr& sub_mgr, bool open_right_tap = true);
	
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

protected:
	sm::vec2 m_first_pos, m_last_pos;

}; // DrawSelectRectOP

}