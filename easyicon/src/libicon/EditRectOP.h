#ifndef _EASYICON_EDIT_RECT_OP_H_
#define _EASYICON_EDIT_RECT_OP_H_

#include <drag2d.h>

namespace eicon
{

class StagePanel;

class EditRectOP : public d2d::ZoomViewOP
{
public:
	EditRectOP(StagePanel* stage);

	virtual bool onActive();

private:
	StagePanel* m_stage;

}; // EditRectOP

}

#endif // _EASYICON_EDIT_RECT_OP_H_