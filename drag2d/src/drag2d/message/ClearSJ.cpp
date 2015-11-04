#include "ClearSJ.h"
#include "subject_id.h"

namespace d2d
{

ClearSJ* ClearSJ::m_instance = NULL;

ClearSJ::ClearSJ(int id)
	: Subject(id)
{
}

void ClearSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

ClearSJ* ClearSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ClearSJ(CLEAR);
	}
	return m_instance;
}

}