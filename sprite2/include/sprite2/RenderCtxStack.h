#ifndef _SPRITE2_RENDER_CONTEXT_STACK_H_
#define _SPRITE2_RENDER_CONTEXT_STACK_H_

#include "RenderCtx.h"

#include <CU_Singleton.h>

#include <vector>

namespace s2
{

class RenderCtxStack
{
public:
	void Push(const RenderCtx& ctx);
	void Pop();

	const RenderCtx* Top() const;
	
private:
	static void BindCtx(const RenderCtx& ctx);

private:
	std::vector<RenderCtx> m_stack;

	SINGLETON_DECLARATION(RenderCtxStack)

}; // RenderCtxStack

}

#endif // _SPRITE2_RENDER_CONTEX_STACK_H_