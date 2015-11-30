#include "EditRedoSJ.h"
#include "subject_id.h"

namespace d2d
{

EditRedoSJ* EditRedoSJ::m_instance = NULL;

EditRedoSJ::EditRedoSJ(int id)
	: Subject(id)
{
}

void EditRedoSJ::Undo()
{
	Notify(NULL);
}

EditRedoSJ* EditRedoSJ::Instance()
{
	if (!m_instance) {
		m_instance = new EditRedoSJ(MSG_EDIT_REDO);
	}
	return m_instance;
}

}