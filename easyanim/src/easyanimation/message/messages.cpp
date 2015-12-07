#include "messages.h"

namespace eanim
{

//////////////////////////////////////////////////////////////////////////
// layer
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(InsertLayerSJ, MSG_INSERT_LAYER)
void InsertLayerSJ::Insert()
{
	Notify(NULL);
}
void InsertLayerSJ::Insert(Layer* layer)
{
	Notify(layer);
}

SUBJECT_DEFINITION(RemoveLayerSJ, MSG_REMOVE_LAYER)
void RemoveLayerSJ::Remove(int layer)
{
	Notify(&layer);
}

SUBJECT_DEFINITION(ReorderLayerSJ, MSG_REORDER_LAYER)
void ReorderLayerSJ::Reorder(int from, int to)
{
	Params p;
	p.from = from;
	p.to = to;
	Notify(&p);
}

//////////////////////////////////////////////////////////////////////////
// fps
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(GetFpsSJ, MSG_GET_FPS)
int GetFpsSJ::Get()
{
	int fps = 0;
	Notify(&fps);
	return fps;
}

SUBJECT_DEFINITION(SetFpsSJ, MSG_SET_FPS)
void SetFpsSJ::Set(int fps)
{
	Notify(&fps);
}

//////////////////////////////////////////////////////////////////////////
// selected
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(SetSelectedSJ, MSG_SET_CURR_FRAME)
void SetSelectedSJ::Set(int layer, int frame)
{
	Position cf;
	cf.layer = layer;
	cf.frame = frame;
	Notify(&cf);
}

SUBJECT_DEFINITION(SetSelectedRegionSJ, MSG_SET_SELECTED_REGION)
void SetSelectedRegionSJ::Set(int col)
{
	Notify(&col);
}

}