#ifndef _EASYEDITOR_PANEL_MSG_H_
#define _EASYEDITOR_PANEL_MSG_H_

#include "Subject.h"

namespace ee
{

//////////////////////////////////////////////////////////////////////////
// tot
//////////////////////////////////////////////////////////////////////////

class ClearPanelSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);
	SUBJECT_DECLARATION(ClearPanelSJ)
}; // ClearPanelSJ

class RefreshPanelSJ : public Subject
{
public:
	void Refresh();
	SUBJECT_DECLARATION(RefreshPanelSJ)
}; // RefreshPanelSJ

class SetCanvasDirtySJ : public Subject
{
public:
	void SetDirty();
	SUBJECT_DECLARATION(SetCanvasDirtySJ)
}; // SetCanvasDirtySJ

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

class AtomicOP;
class EditAddRecordSJ : public Subject
{
public:
	void Add(AtomicOP* op);
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

class EditRedoTopSJ : public Subject
{
public:
	void RedoTop();
	SUBJECT_DECLARATION(EditRedoTopSJ)
}; // EditRedoTopSJ

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

#endif // _EASYEDITOR_PANEL_MSG_H_