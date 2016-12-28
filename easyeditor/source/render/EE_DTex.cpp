#include "EE_DTex.h"
#include "RenderContextStack.h"

#include <SM_Vector.h>
#include <gum/GUM_DTex.h>

namespace ee
{

static sm::vec2 LAST_OFFSET;
static float LAST_SCALE;
static int LAST_WIDTH, LAST_HEIGHT;

static void _draw_begin() 
{
	RenderContextStack* ctx_stack = RenderContextStack::Instance();

	ctx_stack->GetModelView(LAST_OFFSET, LAST_SCALE);
	ctx_stack->GetProjection(LAST_WIDTH, LAST_HEIGHT);

	ctx_stack->SetModelView(sm::vec2(0, 0), 1);
	ctx_stack->SetProjection(2, 2);
}

static void _draw_end()
{
	RenderContextStack* ctx_stack = RenderContextStack::Instance();
	ctx_stack->SetModelView(LAST_OFFSET, LAST_SCALE);
	ctx_stack->SetProjection(LAST_WIDTH, LAST_HEIGHT);
}

void DTex::Init()
{
	gum::DTex::Instance()->InitHook(_draw_begin, _draw_end);	
}

}