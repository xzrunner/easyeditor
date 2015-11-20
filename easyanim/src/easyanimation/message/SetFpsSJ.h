#ifndef _EASYANIM_SET_FPS_SJ_H_
#define _EASYANIM_SET_FPS_SJ_H_

#include <drag2d.h>

namespace eanim
{

class SetFpsSJ : public d2d::Subject
{
public:
	void Set(int fps);

	static SetFpsSJ* Instance();

private:
	SetFpsSJ(int id);

private:
	static SetFpsSJ* m_instance;

}; // SetFpsSJ

}

#endif // _EASYANIM_SET_FPS_SJ_H_