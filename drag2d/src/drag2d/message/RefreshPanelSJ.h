#ifndef _DRAG2D_REFRESH_PANEL_SJ_H_
#define _DRAG2D_REFRESH_PANEL_SJ_H_

#include "Subject.h"

namespace d2d
{

class RefreshPanelSJ : public Subject
{
public:
	void Refresh();

	static RefreshPanelSJ* Instance();

private:
	RefreshPanelSJ(int id);

private:
	static RefreshPanelSJ* m_instance;

}; // RefreshPanelSJ

}

#endif // _DRAG2D_REFRESH_PANEL_SJ_H_