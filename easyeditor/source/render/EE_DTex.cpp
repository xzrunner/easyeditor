#include "EE_DTex.h"

#include <SM_Vector.h>
#include <sprite2/RenderCtxStack.h>
#include <gum/GUM_DTex.h>

namespace ee
{

static void _draw_begin() 
{
	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(2, 2, 0, 0));
}

static void _draw_end()
{
	s2::RenderCtxStack::Instance()->Pop();
}

void DTex::Init()
{
	gum::DTex::Instance()->InitHook(_draw_begin, _draw_end);	
}

}