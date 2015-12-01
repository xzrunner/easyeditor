#ifndef _EASYANIM_SET_SELECTED_REGION_SJ_H_
#define _EASYANIM_SET_SELECTED_REGION_SJ_H_

#include <drag2d.h>

namespace eanim
{

class SetSelectedRegionSJ : public d2d::Subject
{
public:
	struct SelectedRegion
	{
		int row;
		int col_min, col_max;
	};

public:
	void Set(int row, int col_min, int col_max);

	static SetSelectedRegionSJ* Instance();

private:
	SetSelectedRegionSJ(int id);

private:
	static SetSelectedRegionSJ* m_instance;

}; // SetSelectedRegionSJ

}

#endif // _EASYANIM_SET_SELECTED_REGION_SJ_H_