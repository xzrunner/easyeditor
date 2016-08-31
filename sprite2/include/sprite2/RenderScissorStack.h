#ifndef _SPRITE2_RENDER_SCISSOR_STACK_H_
#define _SPRITE2_RENDER_SCISSOR_STACK_H_

#include <CU_Singleton.h>

#include <vector>

namespace s2
{

class RenderScissorStack
{
public:
	void Push(float x, float y, float w, float h);
	void Pop();

public:
	struct Rect
	{
		float x, y, w, h;
	};

private:
	std::vector<Rect> m_stack;
	
	SINGLETON_DECLARATION(RenderScissorStack)

}; // RenderScissorStack

}

#endif // _SPRITE2_RENDER_SCISSOR_STACK_H_