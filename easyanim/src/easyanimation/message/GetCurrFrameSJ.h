#ifndef _EASYANIM_GET_CURR_FRAME_SJ_H_
#define _EASYANIM_GET_CURR_FRAME_SJ_H_

#include <drag2d.h>

namespace eanim
{

class GetCurrFrameSJ : public d2d::Subject
{
public:
	struct CurrFrame
	{
		int layer;
		int frame;
	};

public:
	void Get(int& layer, int& frame);

	static GetCurrFrameSJ* Instance();

private:
	GetCurrFrameSJ(int id);

private:
	static GetCurrFrameSJ* m_instance;

}; // GetCurrFrameSJ

}

#endif // _EASYANIM_GET_CURR_FRAME_SJ_H_