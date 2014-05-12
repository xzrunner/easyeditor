#pragma once

#include <wx/wx.h>
#include <vector>

namespace d2d
{

class EditPanel;
class LibraryPanel;
class ISprite;

class EJPreviewDialog : public wxDialog
{
public:
	EJPreviewDialog(wxWindow* parent,
		const std::vector<const ISprite*>& sprites);
	~EJPreviewDialog();

private:
	void initLayout();

private:
	EditPanel* m_stage;
	LibraryPanel* m_library;

	const std::vector<const ISprite*>& m_sprites;

}; // EJPreviewDialog

}