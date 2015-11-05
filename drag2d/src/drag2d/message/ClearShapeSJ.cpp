#include "ClearShapeSJ.h"
#include "subject_id.h"

namespace d2d
{

ClearShapeSJ* ClearShapeSJ::m_instance = NULL;

ClearShapeSJ::ClearShapeSJ(int id)
	: Subject(id)
{
}

void ClearShapeSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

ClearShapeSJ* ClearShapeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ClearShapeSJ(MSG_CLEAR_SHAPE);
	}
	return m_instance;
}

}