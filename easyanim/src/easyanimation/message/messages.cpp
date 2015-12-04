#include "messages.h"

namespace eanim
{

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
// curr frame
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(SetCurrFrameSJ, MSG_SET_CURR_FRAME)
void SetCurrFrameSJ::Set(int layer, int frame)
{
	CurrFrame cf;
	cf.layer = layer;
	cf.frame = frame;
	Notify(&cf);
}

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
void RemoveLayerSJ::Remove()
{
	Notify(NULL);
}

//////////////////////////////////////////////////////////////////////////
// region
//////////////////////////////////////////////////////////////////////////

SUBJECT_DEFINITION(SetSelectedRegionSJ, MSG_SET_SELECTED_REGION)
void SetSelectedRegionSJ::Set(int row, int col)
{
	Region sr;
	sr.row = row;
	sr.col = col;
	Notify(&sr);
}

SUBJECT_DEFINITION(UpdateSelectedRegionSJ, MSG_UPDATE_SELECTED_REGION)
void UpdateSelectedRegionSJ::Update(int min, int max)
{
	Region sr;
	sr.min = min;
	sr.max = max;
	Notify(&sr);
}


}