#ifndef _EASYIMAGE_STAGE_PANEL_H_
#define _EASYIMAGE_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/CombinedDropTarget.h>

namespace ee { class LibraryPanel; class Symbol; }

namespace eimage
{

class StagePanel : public ee::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);
	virtual ~StagePanel();

	const ee::Sprite* GetImage() const {
		return m_image;
	}
	void SetImage(const std::string& filepath);

	const ee::Sprite* GetLeft() const { return m_left; }
	const ee::Sprite* GetRight() const { return m_right; }

private:
	void SetImage(ee::Symbol* symbol);

private:
	class StageDropTarget : public ee::CombinedDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

	private:
		StagePanel* m_stage;
		ee::LibraryPanel* m_library;

	}; // StageDropTarget

private:
	ee::Sprite* m_image;

	// todo 根据OP的不同来组织数据
	const ee::Sprite *m_left, *m_right;
	
}; // StagePanel

}

#endif // _EASYIMAGE_STAGE_PANEL_H_
