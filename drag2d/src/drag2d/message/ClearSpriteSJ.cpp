#include "ClearSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

ClearSpriteSJ* ClearSpriteSJ::m_instance = NULL;

ClearSpriteSJ::ClearSpriteSJ(int id)
	: Subject(id)
{
}

void ClearSpriteSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

ClearSpriteSJ* ClearSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ClearSpriteSJ(MSG_CLEAR_SPRITE);
	}
	return m_instance;
}

}