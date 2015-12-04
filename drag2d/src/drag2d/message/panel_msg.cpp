#include "panel_msg.h"
#include "subject_id.h"

namespace d2d
{

//////////////////////////////////////////////////////////////////////////
// stage
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(EditAddRecordSJ, MSG_EDIT_ADD_RECORD)
void EditAddRecordSJ::Add(AbstractAtomicOP* op)
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

//////////////////////////////////////////////////////////////////////////
// property
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(SetPropertySettingSJ, MSG_SET_PROPERTY_SETTING)
void SetPropertySettingSJ::Set(IPropertySetting* setting)
{
	Notify(setting);
}

}