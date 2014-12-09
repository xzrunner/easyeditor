#ifndef _ECOMPLEX_RECT_CUT_LOADER_H_
#define _ECOMPLEX_RECT_CUT_LOADER_H_

#include <wx/wx.h>

namespace ecomplex
{

class StagePanel;

class RectCutLoader
{
public:
	RectCutLoader(StagePanel* stage);
	
	void Load(const wxString& pack_file, const wxString& img_name);

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
	StagePanel* m_stage;

}; // RectCutLoader

}

#endif // _ECOMPLEX_RECT_CUT_LOADER_H_