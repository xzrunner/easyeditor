#ifndef _DRAG2D_RESET_VIEWPORT_SJ_H_
#define _DRAG2D_RESET_VIEWPORT_SJ_H_

#include "Subject.h"

namespace d2d
{

class ResetViewportSJ : public Subject
{
public:
	void Reset();

	static ResetViewportSJ* Instance();

private:
	ResetViewportSJ(int id);

private:
	static ResetViewportSJ* m_instance;

}; // ResetViewportSJ

}

#endif // _DRAG2D_RESET_VIEWPORT_SJ_H_