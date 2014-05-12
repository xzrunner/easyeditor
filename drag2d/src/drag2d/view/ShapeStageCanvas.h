#pragma once

#include "OrthoCanvas.h"

namespace d2d
{
	class EditPanel;
	class MultiShapesImpl;

	class ShapeStageCanvas : public OrthoCanvas
	{
	public:
		ShapeStageCanvas(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
			const Colorf& color = Colorf(0.0f, 0.0f, 0.0f));

	protected:
		virtual void onDraw();

	protected:
		Colorf m_color;

		MultiShapesImpl* m_shapesImpl;

	}; // ShapeStageCanvas
}

