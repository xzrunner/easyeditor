#ifndef _EASYICON_EDIT_CHANGED_RECT_OP_H_
#define _EASYICON_EDIT_CHANGED_RECT_OP_H_

#include <drag2d.h>

namespace eicon
{

class StagePanel;

class EditChangedRectOP : public d2d::ZoomViewOP
{
public:
	EditChangedRectOP(StagePanel* stage);

	virtual bool onActive();

private:
	StagePanel* m_stage;

}; // EditChangedRectOP

}

#endif // _EASYICON_EDIT_CHANGED_RECT_OP_H_