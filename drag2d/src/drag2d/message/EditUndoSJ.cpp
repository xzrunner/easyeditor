#include "EditUndoSJ.h"
#include "subject_id.h"

namespace d2d
{

EditUndoSJ* EditUndoSJ::m_instance = NULL;

EditUndoSJ::EditUndoSJ(int id)
	: Subject(id)
{
}

void EditUndoSJ::Undo()
{
	Notify(NULL);
}

EditUndoSJ* EditUndoSJ::Instance()
{
	if (!m_instance) {
		m_instance = new EditUndoSJ(MSG_EDIT_UNDO);
	}
	return m_instance;
}

}