#ifndef _D2D_RENDER_CONTEXT_STACK_H_
#define _D2D_RENDER_CONTEXT_STACK_H_

#include <vector>

namespace d2d
{

class IStageCanvas;
class RenderContext;
class Vector;

class RenderContextStack
{
public:
	void Push(IStageCanvas* canvas, RenderContext* render);
	void Pop();

	void SetModelView(const Vector& offset, float scale);
	void SetProjection(int width, int height);

	bool GetModelView(Vector& offset, float& scale) const;
	bool GetProjection(int& width, int& height) const;

	static RenderContextStack* Instance();

private:
	RenderContextStack() {}
	
private:
	struct Context
	{
		IStageCanvas* canvas;
		RenderContext* render;
	};

private:
	std::vector<Context> m_stack;

private:
	static RenderContextStack* m_instance;

}; // RenderContextStack

}

#endif // _D2D_RENDER_CONTEXT_STACK_H_