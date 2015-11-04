#include "SelectShapeSetSJ.h"
#include "subject_id.h"

namespace d2d
{

SelectShapeSetSJ* SelectShapeSetSJ::m_instance = NULL;

SelectShapeSetSJ::SelectShapeSetSJ(int id)
	: Subject(id)
{
}

void SelectShapeSetSJ::Selecte(const ShapeSelection* selection, Observer* except)
{
	Notify((void*)selection, except);
}

SelectShapeSetSJ* SelectShapeSetSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SelectShapeSetSJ(SELECT_SHAPE_SET);
	}
	return m_instance;
}

}