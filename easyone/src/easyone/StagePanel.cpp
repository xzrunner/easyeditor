#include "StagePanel.h"

#include <ee0/StagePage.h>

namespace eone
{

StagePanel::StagePanel(wxWindow* parent)
	: wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER)
{
}

ee0::StagePage* StagePanel::GetCurrentStagePage() const
{
	return dynamic_cast<ee0::StagePage*>(GetCurrentPage());
}

}