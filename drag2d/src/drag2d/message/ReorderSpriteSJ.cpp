#include "ReorderSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

ReorderSpriteSJ* ReorderSpriteSJ::m_instance = NULL;

ReorderSpriteSJ::ReorderSpriteSJ(int id)
	: Subject(id)
{
}

void ReorderSpriteSJ::Reorder(const Params& p, Observer* except)
{
	Notify((void*)&p, except);
}

ReorderSpriteSJ* ReorderSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ReorderSpriteSJ(REORDER_SPRITE);
	}
	return m_instance;
}

}