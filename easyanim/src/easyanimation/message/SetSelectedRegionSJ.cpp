#include "SetSelectedRegionSJ.h"
#include "message_id.h"

namespace eanim
{

SetSelectedRegionSJ* SetSelectedRegionSJ::m_instance = NULL;

SetSelectedRegionSJ::SetSelectedRegionSJ(int id)
	: Subject(id)
{
}

void SetSelectedRegionSJ::Set(int row, int col_min, int col_max)
{
	SelectedRegion sr;
	sr.row = row;
	sr.col_min = col_min;
	sr.col_max = col_max;
	Notify(&sr);
}

SetSelectedRegionSJ* SetSelectedRegionSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SetSelectedRegionSJ(MSG_SET_SELECTED_REGION);
	}
	return m_instance;
}

}