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

	const d2d::ISprite* getLeft() const { return m_left; }
	const d2d::ISprite* getRight() const { return m_right; }

private:
	void setImage(d2d::ISymbol* symbol);

private:
	class StageDropTarget : public d2d::CombinedDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

	private:
		StagePanel* m_stage;
		d2d::LibraryPanel* m_library;

	}; // StageDropTarget

private:
	d2d::ISprite* m_image;

	// todo 根据OP的不同来组织数据
	const d2d::ISprite *m_left, *m_right;
	
}; // StagePanel

}

#endif // _EASYIMAGE_STAGE_PANEL_H_
