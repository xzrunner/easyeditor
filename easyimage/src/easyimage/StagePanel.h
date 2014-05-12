#ifndef _EASYIMAGE_STAGE_PANEL_H_
#define _EASYIMAGE_STAGE_PANEL_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel : public d2d::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	const d2d::ISprite* getImage() const {
		return m_image;
	}
	void setImage(const std::string& filepath);

private:
	void setImage(d2d::ISymbol* symbol);

private:
	class DragSymbolTarget : public wxTextDropTarget
	{
	public:
		DragSymbolTarget(StagePanel* stage, d2d::LibraryPanel* library);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		StagePanel* m_stage;
		d2d::LibraryPanel* m_library;

	}; // DragSymbolTarget

private:
	const d2d::ISprite* m_image;

}; // StagePanel

}

#endif // _EASYIMAGE_STAGE_PANEL_H_
