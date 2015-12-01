#ifndef _EASYANIM_GET_SELECTED_REGION_SJ_H_
#define _EASYANIM_GET_SELECTED_REGION_SJ_H_

#include <drag2d.h>

namespace eanim
{

class GetSelectedRegionSJ : public d2d::Subject
{
public:
	struct SelectedRegion
	{
		int row;
		int col_min, col_max;
	};

public:
	void Get(int& row, int& col_min, int& col_max);

	static GetSelectedRegionSJ* Instance();

private:
	GetSelectedRegionSJ(int id);

private:
	static GetSelectedRegionSJ* m_instance;

}; // GetSelectedRegionSJ

}

#endif // _EASYANIM_GET_SELECTED_REGION_SJ_H_