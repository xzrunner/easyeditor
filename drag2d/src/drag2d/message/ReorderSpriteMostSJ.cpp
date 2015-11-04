#include "ReorderSpriteMostSJ.h"
#include "subject_id.h"

namespace d2d
{

ReorderSpriteMostSJ* ReorderSpriteMostSJ::m_instance = NULL;

ReorderSpriteMostSJ::ReorderSpriteMostSJ(int id)
	: Subject(id)
{
}

void ReorderSpriteMostSJ::Reorder(const Params& p, Observer* except)
{
	Notify((void*)&p, except);
}

ReorderSpriteMostSJ* ReorderSpriteMostSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ReorderSpriteMostSJ(REORDER_SPRITE_MOST);
	}
	return m_instance;
}

}