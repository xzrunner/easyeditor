#ifndef EANIM_SKELETON_CMPT_H
#define EANIM_SKELETON_CMPT_H

#include <drag2d.h>

namespace eanim
{
	class StagePanel;

	class SkeletonCMPT : public d2d::AbstractEditCMPT
	{
	public:
		SkeletonCMPT(wxWindow* parent, const wxString& name,
			StagePanel* stage, bool vertical);

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSizer* initEditPanel();

	}; // SkeletonCMPT
}

#endif // EANIM_SKELETON_CMPT_H