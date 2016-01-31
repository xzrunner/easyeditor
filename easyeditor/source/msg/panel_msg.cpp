#include "panel_msg.h"
#include "subject_id.h"

namespace ee
{

//////////////////////////////////////////////////////////////////////////
// tot
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(ClearPanelSJ, MSG_CLEAR_PANEL)
void ClearPanelSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

SUBJECT_DEFINITION(RefreshPanelSJ, MSG_REFRESH_PANEL)
void RefreshPanelSJ::Refresh()
{
	Notify(NULL);
}

SUBJECT_DEFINITION(SetCanvasDirtySJ, MSG_SET_CANVAS_DIRTY)
void SetCanvasDirtySJ::SetDirty()
{
	Notify(NULL);
}

//////////////////////////////////////////////////////////////////////////
// layer
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(ChangeLayerMgrSJ, MSG_CHANGE_LAYER_MGR_MSG)
void ChangeLayerMgrSJ::Change(LayerMgr* mgr)
{
	Notify(mgr);
}

//////////////////////////////////////////////////////////////////////////
// stage
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(EditAddRecordSJ, MSG_EDIT_ADD_RECORD)
void EditAddRecordSJ::Add(AtomicOP* op)
{
	Notify(op);
}

SUBJECT_DEFINITION(EditRedoSJ, MSG_EDIT_REDO)
void EditRedoSJ::Redo()
{
	Notify(NULL);
}

SUBJECT_DEFINITION(EditUndoSJ, MSG_EDIT_UNDO)
void EditUndoSJ::Undo()
{
	Notify(NULL);
}

SUBJECT_DEFINITION(GetKeyStateSJ, MSG_GET_KEY_STATE)
bool GetKeyStateSJ::Query(int key)
{
	State st;
	st.key = key;
	st.state = false;
	Notify(&st);
	return st.state;
}

SUBJECT_DEFINITION(SetWndDirtySJ, MSG_SET_WND_DIRTY)
void SetWndDirtySJ::SetDirty()
{
	Notify(NULL);
}

}