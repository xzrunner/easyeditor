#include "RemoveShapeSJ.h"
#include "subject_id.h"

namespace d2d
{

RemoveShapeSJ* RemoveShapeSJ::m_instance = NULL;

RemoveShapeSJ::RemoveShapeSJ(int id)
	: Subject(id)
{
}

void RemoveShapeSJ::Remove(Shape* spr, Observer* except)
{
	Notify((void*)spr, except);
}

RemoveShapeSJ* RemoveShapeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new RemoveShapeSJ(MSG_REMOVE_SHAPE);
	}
	return m_instance;
}

}