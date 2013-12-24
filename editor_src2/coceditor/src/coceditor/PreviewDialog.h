#pragma once

#include <drag2d.h>

namespace coceditor
{
	class PreviewDialog : public wxDialog
	{
	public:
		PreviewDialog(wxWindow* parent, d2d::LibraryPanel* library,
			const std::vector<d2d::ISprite*>& sprites);
		~PreviewDialog();

	private:
		void initLayout();

		void buildEditPanel(wxSizer* topSizer);

	private:
		d2d::EditPanel* m_stage;
		d2d::LibraryPanel* m_library;

		const std::vector<d2d::ISprite*>& m_sprites;

	}; // PreviewDialog
}

