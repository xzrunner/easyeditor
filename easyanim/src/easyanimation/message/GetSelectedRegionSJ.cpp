#include "GetSelectedRegionSJ.h"
#include "message_id.h"

namespace eanim
{

GetSelectedRegionSJ* GetSelectedRegionSJ::m_instance = NULL;

GetSelectedRegionSJ::GetSelectedRegionSJ(int id)
	: Subject(id)
{
}

void GetSelectedRegionSJ::Get(int& row, int& col_min, int& col_max)
{
	SelectedRegion sr;
	Notify(&sr);
	row = sr.row;
	col_min = sr.col_min;
	col_max = sr.col_max;
}

GetSelectedRegionSJ* GetSelectedRegionSJ::Instance()
{
	if (!m_instance) {
		m_instance = new GetSelectedRegionSJ(MSG_GET_SELECTED_REGION);
	}
	return m_instance;
}

}