#ifndef _DRAG2D_EDIT_REDO_SJ_H_
#define _DRAG2D_EDIT_REDO_SJ_H_

#include "Subject.h"

namespace d2d
{

class EditRedoSJ : public Subject
{
public:
	void Undo();

	static EditRedoSJ* Instance();

private:
	EditRedoSJ(int id);

private:
	static EditRedoSJ* m_instance;

}; // EditRedoSJ

}

#endif // _DRAG2D_EDIT_REDO_SJ_H_