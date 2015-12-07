#ifndef _EASYANIM_FPS_MSG_H_
#define _EASYANIM_FPS_MSG_H_

#include <drag2d.h>

namespace eanim
{

enum MSG_ID {
	MSG_INSERT_LAYER = 1000,
	MSG_REMOVE_LAYER,
	MSG_REORDER_LAYER,

	MSG_GET_FPS,
	MSG_SET_FPS,

	MSG_SET_CURR_FRAME,
	MSG_SET_SELECTED_REGION,
};

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
	void Remove(int layer);
	SUBJECT_DECLARATION(RemoveLayerSJ)
}; // RemoveLayerSJ

class ReorderLayerSJ : public d2d::Subject
{
public:
	struct Params
	{
		int from, to;
	};
public:
	void Reorder(int from, int to);
	SUBJECT_DECLARATION(ReorderLayerSJ)
}; // ReorderLayerSJ

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
// selected
//////////////////////////////////////////////////////////////////////////

class SetSelectedSJ : public d2d::Subject
{
public:
	struct Position
	{
		int layer;
		int frame;
	};
public:
	void Set(int layer, int frame);
	SUBJECT_DECLARATION(SetSelectedSJ)
}; // SetSelectedSJ

class SetSelectedRegionSJ : public d2d::Subject
{
public:
	void Set(int col);
	SUBJECT_DECLARATION(SetSelectedRegionSJ)
}; // SetSelectedRegionSJ

}

#endif // _EASYANIM_FPS_MSG_H_