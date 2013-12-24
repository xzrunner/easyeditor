#pragma once

#include "GLCanvas.h"

namespace d2d
{
	class EditPanel;

	class PerspectCanvas : public GLCanvas
	{
	public:
		PerspectCanvas(EditPanel* editPanel);

	private:
		virtual void onSize(int w, int h);

	}; // PerspectCanvas
}

