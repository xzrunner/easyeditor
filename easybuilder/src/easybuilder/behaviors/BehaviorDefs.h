
#ifndef EBUILDER_BEHAVIOR_DEFINES_H
#define EBUILDER_BEHAVIOR_DEFINES_H

#include <wx/wx.h>

namespace ebuilder
{
	enum BehaviorType
	{
		e_ChangeImage = 0,
		e_ChangeScene,
		e_ScaleSize,

		e_MouseButton,

		e_Move,
		e_Rotate
	};

	const wxString CHANGE_IMAGE_NAME = wxT("Change Image");
	const wxString CHANGE_SCENE_NAME = wxT("Change Scene");
	const wxString SCALE_SIZE_NAME = wxT("Scale Size");

	const wxString MOUSE_BUTTON_NAME = wxT("Mouse Button");

	const wxString MOVE_NAME = wxT("Move");
	const wxString ROTATE_NAME = wxT("Rotate");
}

#endif // EBUILDER_BEHAVIOR_DEFINES_H
