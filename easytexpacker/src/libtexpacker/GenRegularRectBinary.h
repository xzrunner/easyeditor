#ifndef _LIBTEXPACKER_GEN_REGULAR_RECT_BINARARY_H_
#define _LIBTEXPACKER_GEN_REGULAR_RECT_BINARARY_H_

#include <wx/wx.h>
#include <vector>

namespace libtexpacker
{

class GenRegularRectBinary
{
public:
	GenRegularRectBinary(const wxString& json_file, 
		const wxString& img_id_file);
	~GenRegularRectBinary();

	void PackToBinary() const;

private:
	void LoadRegularRectPackFile(const wxString& json_file, 
		const wxString& img_id_file);

private:
	struct Rect
	{
		int w, h;
		int x, y;
	};

	struct Part
	{
		Rect src, dst;
		wxString filepath;
	};

	struct PartCmp
	{
		bool operator () (const Part* lhs, const Part* rhs) const {
			return lhs->filepath.compare(rhs->filepath) < 0;
		}
	};

	struct Picture
	{
		std::vector<Part*> parts;
		int w, h;

		wxString path;
		int id;
	};

private:
	wxString m_filepath;

	std::vector<Picture*> m_pics;

}; // GenRegularRectBinary

}

#endif // _LIBTEXPACKER_GEN_REGULAR_RECT_BINARARY_H_