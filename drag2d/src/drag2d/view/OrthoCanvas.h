#pragma once

#include "GLCanvas.h"

namespace d2d
{
	class EditPanel;

	class OrthoCanvas : public GLCanvas
	{
	public:
		OrthoCanvas(EditPanel* editPanel);

	protected:
		virtual void onSize(int w, int h);

	}; // OrthoCanvas
}

