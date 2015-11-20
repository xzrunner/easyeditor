#ifndef _EASYANIM_GET_FPS_SJ_H_
#define _EASYANIM_GET_FPS_SJ_H_

#include <drag2d.h>

namespace eanim
{

class GetFpsSJ : public d2d::Subject
{
public:
	int Get();

	static GetFpsSJ* Instance();

private:
	GetFpsSJ(int id);

private:
	static GetFpsSJ* m_instance;

}; // GetFpsSJ

}

#endif // _EASYANIM_GET_FPS_SJ_H_