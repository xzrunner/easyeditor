#pragma once

#include "dataset/WholeSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class WholeSkeleton;
	class SkeletonSymbolList;

	class SkeletonEditGLCanvas : public d2d::OrthoCanvas
	{
	public:
		SkeletonEditGLCanvas(d2d::EditPanel* editPanel, WholeSkeleton* skeleton, 
			SkeletonSymbolList* symbolList);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);

	private:
		WholeSkeleton* m_skeleton;
		SkeletonSymbolList* m_symbolList;

		DECLARE_EVENT_TABLE()

	}; // SkeletonEditGLCanvas
}

