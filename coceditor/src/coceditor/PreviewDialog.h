#pragma once



namespace coceditor
{
	class PreviewDialog : public wxDialog
	{
	public:
		PreviewDialog(wxWindow* parent, ee::LibraryPanel* library,
			const std::vector<ee::ISprite*>& sprites);
		~PreviewDialog();

	private:
		void initLayout();

		void buildEditPanel(wxSizer* topSizer);

	private:
		ee::EditPanel* m_stage;
		ee::LibraryPanel* m_library;

		const std::vector<ee::ISprite*>& m_sprites;

	}; // PreviewDialog
}

