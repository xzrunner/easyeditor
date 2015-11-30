#ifndef _EASYANIM_SET_CURR_FRAME_SJ_H_
#define _EASYANIM_SET_CURR_FRAME_SJ_H_

#include <drag2d.h>

namespace eanim
{

class SetCurrFrameSJ : public d2d::Subject
{
public:
	struct CurrFrame
	{
		int layer;
		int frame;
	};

public:
	void Set(int layer, int frame);

	static SetCurrFrameSJ* Instance();

private:
	SetCurrFrameSJ(int id);

private:
	static SetCurrFrameSJ* m_instance;

}; // SetCurrFrameSJ

}

#endif // _EASYANIM_SET_CURR_FRAME_SJ_H_