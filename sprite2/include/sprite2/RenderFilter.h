#ifndef _SPRITE2_RENDER_FILTER_H_
#define _SPRITE2_RENDER_FILTER_H_

#include "FilterMode.h"

#include <CU_Uncopyable.h>

namespace s2
{

class RenderFilter : public cu::Uncopyable
{
public:
	RenderFilter(FilterMode mode) 
		: m_mode(mode) 
	{}

	FilterMode GetMode() const { return m_mode; }

private:
	FilterMode m_mode;

}; // RenderFilter

}

#endif // _SPRITE2_RENDER_FILTER_H_