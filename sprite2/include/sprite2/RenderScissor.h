#ifndef _SPRITE2_RENDER_SCISSOR_H_
#define _SPRITE2_RENDER_SCISSOR_H_

#include <CU_Singleton.h>

#include <vector>

namespace s2
{

class RenderScissor
{
public:
	void Push(float x, float y, float w, float h);
	void Pop();

	void Close();
	void Open();

public:
	struct Rect
	{
		float x, y, w, h;
	};

private:
	std::vector<Rect> m_stack;
	
	SINGLETON_DECLARATION(RenderScissor)

}; // RenderScissor

}

#endif // _SPRITE2_RENDER_SCISSOR_H_