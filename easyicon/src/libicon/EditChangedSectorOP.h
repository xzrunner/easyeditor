#ifndef _EASYICON_EDIT_CHANGED_SECTOR_OP_H_
#define _EASYICON_EDIT_CHANGED_SECTOR_OP_H_



namespace eicon
{

class StagePanel;

class EditChangedSectorOP : public ee::ZoomViewOP
{
public:
	EditChangedSectorOP(StagePanel* stage);

	virtual bool OnActive();

private:
	StagePanel* m_stage;

}; // EditChangedSectorOP


}

#endif // _EASYICON_EDIT_CHANGED_SECTOR_OP_H_