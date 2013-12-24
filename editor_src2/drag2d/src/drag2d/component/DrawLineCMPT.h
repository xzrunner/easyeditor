#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiShapesImpl;
	class PropertySettingPanel;

	class DrawLineCMPT : public AbstractEditCMPT
	{
	public:
		DrawLineCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel);

	protected:
		virtual wxSizer* initLayout();

	}; // DrawLineCMPT
}

