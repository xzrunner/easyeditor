#ifndef _EASYANIM_FPS_MSG_H_
#define _EASYANIM_FPS_MSG_H_

#include <drag2d.h>

namespace eanim
{

enum MSG_ID {
	MSG_INSERT_LAYER = 1000,
	MSG_REMOVE_LAYER,

	MSG_GET_FPS,
	MSG_SET_FPS,

	MSG_SET_CURR_FRAME,

	MSG_SET_SELECTED_REGION,
	MSG_UPDATE_SELECTED_REGION,

	MSG_CHANGE_TEMPLATE_MODE,
};

//////////////////////////////////////////////////////////////////////////
// fps
//////////////////////////////////////////////////////////////////////////

class GetFpsSJ : public d2d::Subject
{
public:
	int Get();
	SUBJECT_DECLARATION(GetFpsSJ)
}; // GetFpsSJ

class SetFpsSJ : public d2d::Subject
{
public:
	void Set(int fps);
	SUBJECT_DECLARATION(SetFpsSJ)
}; // SetFpsSJ

//////////////////////////////////////////////////////////////////////////
// curr frame
//////////////////////////////////////////////////////////////////////////

class SetCurrFrameSJ : public d2d::Subject
{
public:
	struct CurrFrame
	{
		int layer;
		int frame;
	};
public:
	void Set(int layer, int frame);
	SUBJECT_DECLARATION(SetCurrFrameSJ)
}; // SetCurrFrameSJ

//////////////////////////////////////////////////////////////////////////
// layer
//////////////////////////////////////////////////////////////////////////

class Layer;
class InsertLayerSJ : public d2d::Subject
{
public:
	void Insert();
	void Insert(Layer* layer);
	SUBJECT_DECLARATION(InsertLayerSJ)
}; // InsertLayerSJ

class RemoveLayerSJ : public d2d::Subject
{
public:
	void Remove();
	SUBJECT_DECLARATION(RemoveLayerSJ)
}; // RemoveLayerSJ

//////////////////////////////////////////////////////////////////////////
// region
//////////////////////////////////////////////////////////////////////////

class SetSelectedRegionSJ : public d2d::Subject
{
public:
	struct Region
	{
		int row, col;
	};
public:
	void Set(int row, int col);
	SUBJECT_DECLARATION(SetSelectedRegionSJ)
}; // SetSelectedRegionSJ

class UpdateSelectedRegionSJ : public d2d::Subject
{
public:
	struct Region
	{
		int min, max;
	};
public:
	void Update(int min, int max);
	SUBJECT_DECLARATION(UpdateSelectedRegionSJ)
}; // UpdateSelectedRegionSJ

}

#endif // _EASYANIM_FPS_MSG_H_