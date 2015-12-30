#ifndef _DRAG2D_PANEL_MSG_H_
#define _DRAG2D_PANEL_MSG_H_

#include "Subject.h"

namespace d2d
{

//////////////////////////////////////////////////////////////////////////
// layer
//////////////////////////////////////////////////////////////////////////

class LayerMgr;
class ChangeLayerMgrSJ : public Subject
{
public:
	void Change(LayerMgr* mgr);
	SUBJECT_DECLARATION(ChangeLayerMgrSJ)
}; // ChangeLayerMgrSJ

//////////////////////////////////////////////////////////////////////////
// stage
//////////////////////////////////////////////////////////////////////////

class AbstractAtomicOP;
class EditAddRecordSJ : public Subject
{
public:
	void Add(AbstractAtomicOP* op);
	SUBJECT_DECLARATION(EditAddRecordSJ)
}; // EditAddRecordSJ

class EditRedoSJ : public Subject
{
public:
	void Redo();
	SUBJECT_DECLARATION(EditRedoSJ)
}; // EditRedoSJ

class EditUndoSJ : public Subject
{
public:
	void Undo();
	SUBJECT_DECLARATION(EditUndoSJ)
}; // EditUndoSJ

class GetKeyStateSJ : public Subject
{
public:
	struct State {
		int key;
		bool state;
	};
public:
	bool Query(int key);
	SUBJECT_DECLARATION(GetKeyStateSJ)
}; // GetKeyStateSJ

class SetWndDirtySJ : public Subject
{
public:
	void SetDirty();
	SUBJECT_DECLARATION(SetWndDirtySJ)
}; // SetWndDirtySJ

}

#endif // _DRAG2D_PANEL_MSG_H_