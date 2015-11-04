#include "SelectSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

SelectSpriteSJ* SelectSpriteSJ::m_instance = NULL;

SelectSpriteSJ::SelectSpriteSJ(int id)
	: Subject(id)
{
}

void SelectSpriteSJ::Select(const Params& p, Observer* except)
{
	Notify((void*)&p, except);
}

SelectSpriteSJ* SelectSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SelectSpriteSJ(SELECT_SPRITE);
	}
	return m_instance;
}

}