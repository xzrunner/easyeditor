#ifndef _ECOMPLEX_RECT_CUT_LOADER_H_
#define _ECOMPLEX_RECT_CUT_LOADER_H_

#include <wx/wx.h>
#include <vector>

namespace ecomplex
{

class StagePanel;

class RectCutLoader
{
public:
	RectCutLoader(StagePanel* stage);
	
	void LoadOnlyJson(const wxString& pack_file, const wxString& img_name);

	void LoadJsonAndImg(const wxString& pack_file, const wxString& img_name);

private:
	struct Rect
	{
		int w, h;
		int x, y;
	};

	struct Sprite
	{
		Rect src, dst;
		wxString filepath;
	};

private:
	void LoadJsonFile(const wxString& pack_file, const wxString& img_name,
		std::vector<Sprite>& sprites);

private:
	StagePanel* m_stage;

}; // RectCutLoader

}

#endif // _ECOMPLEX_RECT_CUT_LOADER_H_