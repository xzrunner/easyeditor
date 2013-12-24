#pragma once

#include "OrthoCanvas.h"

namespace d2d
{
	class EditPanel;
	class ISymbol;

	class DialogStageCanvas : public OrthoCanvas
	{
	public:
		DialogStageCanvas(EditPanel* editPanel, ISymbol* symbol);

	protected:
		virtual void initGL();
		virtual void onDraw();

	private:
		ISymbol* m_symbol;

	}; // DialogStageCanvas
}

