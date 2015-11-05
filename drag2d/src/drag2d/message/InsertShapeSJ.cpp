#include "InsertShapeSJ.h"
#include "subject_id.h"

namespace d2d
{

InsertShapeSJ* InsertShapeSJ::m_instance = NULL;

InsertShapeSJ::InsertShapeSJ(int id)
	: Subject(id)
{
}

void InsertShapeSJ::Insert(d2d::IShape* shape, Observer* except)
{
	Notify((void*)shape, except);
}

InsertShapeSJ* InsertShapeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new InsertShapeSJ(MSG_INSERT_SHAPE);
	}
	return m_instance;
}

}