#pragma once

#include <wx/aui/auibook.h>

namespace ee0 { class StagePage; }

namespace eone
{

class StagePanel : public wxAuiNotebook
{
public:
	StagePanel(wxWindow* parent);

	ee0::StagePage* GetCurrentStagePage() const;

}; // StagePanel

}