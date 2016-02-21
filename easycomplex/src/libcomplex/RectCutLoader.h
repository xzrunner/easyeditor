#ifndef _E_EASYCOMPLEX_RECT_CUT_LOADER_H_
#define _E_EASYCOMPLEX_RECT_CUT_LOADER_H_

#include <wx/wx.h>
#include <vector>

namespace ecomplex
{

class RectCutLoader
{
public:	
	void LoadOnlyJson(const std::string& pack_file, const std::string& img_name);

	void LoadJsonAndImg(const std::string& pack_file, const std::string& img_name);

//	void LoadToDtex(const std::string& pack_file, const std::string& img_name);

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
		std::string filepath;
	};

private:
	void LoadJsonFile(const std::string& pack_file, const std::string& img_name,
		std::vector<Picture>& pictures);

	void LoadRRPFile(const std::string& pack_file, int img_id,
		std::vector<Picture>& pictures);

}; // RectCutLoader

}

#endif // _E_EASYCOMPLEX_RECT_CUT_LOADER_H_