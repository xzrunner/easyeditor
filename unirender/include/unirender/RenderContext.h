#ifndef _UNIRENDER_RENDER_CONTEXT_H_
#define _UNIRENDER_RENDER_CONTEXT_H_

#include <CU_Singleton.h>

struct render;

namespace ur
{

class RenderContext
{
public:
	
	render* GetRender() { return m_render; }
	
public:
	static const int MAX_LAYOUT          = 32;
	static const int MAX_TEXTURE         = 1024;
	static const int MAX_TEXTURE_CHANNEL = 8;
	static const int MAX_SHADER          = 64;

private:
	render* m_render;
	
	SINGLETON_DECLARATION(RenderContext);

}; // RenderContext

}

#endif // _UNIRENDER_RENDER_CONTEXT_H_