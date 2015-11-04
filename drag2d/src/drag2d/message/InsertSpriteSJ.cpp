#include "InsertSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

InsertSpriteSJ* InsertSpriteSJ::m_instance = NULL;

InsertSpriteSJ::InsertSpriteSJ(int id)
	: Subject(id)
{
}

void InsertSpriteSJ::Insert(const Params& p, Observer* except)
{
	Notify((void*)&p, except);
}

InsertSpriteSJ* InsertSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new InsertSpriteSJ(INSERT_SPRITE);
	}
	return m_instance;
}

}