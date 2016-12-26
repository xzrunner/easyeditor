#ifndef _SHADERLAB_UTILITY_H_
#define _SHADERLAB_UTILITY_H_

#include <stdint.h>

namespace ur { class RenderContext; }

namespace sl
{

class RenderBuffer;

class Utility
{
public:
	static RenderBuffer* CreateIndexBuffer(ur::RenderContext* rc, int count);
	static RenderBuffer* CreateQuadIndexBuffer(ur::RenderContext* rc, int quad_count);

private:
	static void FillingQuadIndexBuffer(uint16_t* buf, int quad_count);

}; // Utility

}

#endif // _SHADERLAB_UTILITY_H_