#include "RenderContext.h"

namespace ur
{

extern "C"
void ur_set_point_size(void* _rc, float size)
{
	RenderContext* rc = static_cast<RenderContext*>(_rc);
	rc->SetPointSize(size);
}

extern "C"
void ur_set_line_width(void* _rc, float width)
{
	RenderContext* rc = static_cast<RenderContext*>(_rc);
	rc->SetLineWidth(width);
}

extern "C"
void ur_enable_line_stripple(void* _rc, bool stripple)
{
	RenderContext* rc = static_cast<RenderContext*>(_rc);
	rc->EnableLineStripple(stripple);
}

extern "C"
void ur_set_line_stripple(void* _rc, int pattern)
{
	RenderContext* rc = static_cast<RenderContext*>(_rc);
	rc->SetLineStripple(pattern);
}

}