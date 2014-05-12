#pragma once

#include <wx/wx.h>

namespace eanim
{
	class WholeSkeleton;
	class SkeletonEditPanel;

	class SkeletonEditDialog : public wxDialog
	{
	public:
		SkeletonEditDialog(wxWindow* parent, WholeSkeleton* skeleton);
		~SkeletonEditDialog();

	private:
		void onSize(wxSizeEvent& event);

		void initLayout();

		void onLayerChange(wxCommandEvent& event);

	private:
		WholeSkeleton* m_skeleton;
		SkeletonEditPanel* m_editPanel;

		DECLARE_EVENT_TABLE()

	}; // SkeletonEditDialog
}

