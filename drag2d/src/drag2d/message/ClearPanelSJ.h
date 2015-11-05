#ifndef _DRAG2D_CLEAR_PANEL_SJ_H_
#define _DRAG2D_CLEAR_PANEL_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class ClearPanelSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);

	static ClearPanelSJ* Instance();

private:
	ClearPanelSJ(int id);

private:
	static ClearPanelSJ* m_instance;

}; // ClearPanelSJ

}

#endif // _DRAG2D_CLEAR_PANEL_SJ_H_