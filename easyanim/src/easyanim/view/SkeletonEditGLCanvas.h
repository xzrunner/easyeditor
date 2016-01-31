#pragma once

#include "dataset/WholeSkeleton.h"



namespace eanim
{
	class WholeSkeleton;
	class SkeletonSymbolList;

	class SkeletonEditGLCanvas : public ee::OrthoCanvas
	{
	public:
		SkeletonEditGLCanvas(ee::EditPanel* editPanel, WholeSkeleton* skeleton, 
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

