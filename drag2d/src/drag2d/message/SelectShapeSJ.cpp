#include "SelectShapeSJ.h"
#include "subject_id.h"

namespace d2d
{

SelectShapeSJ* SelectShapeSJ::m_instance = NULL;

SelectShapeSJ::SelectShapeSJ(int id)
	: Subject(id)
{
}

void SelectShapeSJ::Select(Shape* shape, Observer* except)
{
	Notify((void*)shape, except);
}

SelectShapeSJ* SelectShapeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SelectShapeSJ(MSG_SELECT_SHAPE);
	}
	return m_instance;
}

}