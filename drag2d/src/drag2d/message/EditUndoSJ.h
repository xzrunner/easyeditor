#ifndef _DRAG2D_EDIT_UNDO_SJ_H_
#define _DRAG2D_EDIT_UNDO_SJ_H_

#include "Subject.h"

namespace d2d
{

class EditUndoSJ : public Subject
{
public:
	void Undo();

	static EditUndoSJ* Instance();

private:
	EditUndoSJ(int id);

private:
	static EditUndoSJ* m_instance;

}; // EditUndoSJ

}

#endif // _DRAG2D_EDIT_UNDO_SJ_H_