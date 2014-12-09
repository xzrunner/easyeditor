#ifndef _RT_RENDER_OUTPUT_H_
#define _RT_RENDER_OUTPUT_H_

namespace rt
{

class RenderOutput
{
public:
	virtual void SetPixel(int x, int y, int red, int green, int blue) = 0;

}; // RenderOutput

}

#endif // _RT_RENDER_OUTPUT_H_