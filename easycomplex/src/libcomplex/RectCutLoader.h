#ifndef _ECOMPLEX_RECT_CUT_LOADER_H_
#define _ECOMPLEX_RECT_CUT_LOADER_H_

#include <wx/wx.h>
#include <vector>

namespace ecomplex
{

class RectCutLoader
{
public:	
	void LoadOnlyJson(const wxString& pack_file, const wxString& img_name);

	void LoadJsonAndImg(const wxString& pack_file, const wxString& img_name);

//	void LoadToDtex(const wxString& pack_file, const wxString& img_name);

private:
	struct Rect
	{
		int w, h;
		int x, y;
		Rect() : w(0), h(0), x(0), y(0) {}
	};

	struct Picture
	{
		Rect src, dst;
		wxString filepath;
	};

private:
	void LoadJsonFile(const wxString& pack_file, const wxString& img_name,
		std::vector<Picture>& pictures);

	void LoadRRPFile(const wxString& pack_file, int img_id,
		std::vector<Picture>& pictures);

}; // RectCutLoader

}

#endif // _ECOMPLEX_RECT_CUT_LOADER_H_