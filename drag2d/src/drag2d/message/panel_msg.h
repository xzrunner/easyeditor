#ifndef _DRAG2D_PANEL_MSG_H_
#define _DRAG2D_PANEL_MSG_H_

#include "Subject.h"

namespace d2d
{

//////////////////////////////////////////////////////////////////////////
// stage
//////////////////////////////////////////////////////////////////////////

class AbstractAtomicOP;
class EditAddRecordSJ : public Subject
{
public:
	void Add(AbstractAtomicOP* op);
	SUBJECT_DECLARATION(EditAddRecordSJ)
}; // EditAddRecordSJ

class EditRedoSJ : public Subject
{
public:
	void Redo();
	SUBJECT_DECLARATION(EditRedoSJ)
}; // EditRedoSJ

class EditUndoSJ : public Subject
{
public:
	void Redo();
	SUBJECT_DECLARATION(EditUndoSJ)
}; // EditUndoSJ

class GetKeyStateSJ : public Subject
{
public:
	struct State {
		int key;
		bool state;
	};
public:
	bool Query(int key);
	SUBJECT_DECLARATION(GetKeyStateSJ)
}; // GetKeyStateSJ

//////////////////////////////////////////////////////////////////////////
// property
//////////////////////////////////////////////////////////////////////////

class IPropertySetting;
class SetPropertySettingSJ : public Subject
{
public:
	void Set(IPropertySetting* setting);
	SUBJECT_DECLARATION(SetPropertySettingSJ)
}; // SetPropertySettingSJ

}

#endif // _DRAG2D_PANEL_MSG_H_