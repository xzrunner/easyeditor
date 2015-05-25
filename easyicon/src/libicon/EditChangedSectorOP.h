#ifndef _EASYICON_EDIT_CHANGED_SECTOR_OP_H_
#define _EASYICON_EDIT_CHANGED_SECTOR_OP_H_

#include <drag2d.h>

namespace eicon
{

class StagePanel;

class EditChangedSectorOP : public d2d::ZoomViewOP
{
public:
	EditChangedSectorOP(StagePanel* stage);

	virtual bool onActive();

private:
	StagePanel* m_stage;

}; // EditChangedSectorOP


}

#endif // _EASYICON_EDIT_CHANGED_SECTOR_OP_H_