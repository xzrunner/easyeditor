#ifndef _EASYTEXPACKER_GEN_REGULAR_RECT_BINARARY_H_
#define _EASYTEXPACKER_GEN_REGULAR_RECT_BINARARY_H_

#include <vector>

namespace etexpacker
{

class GenRegularRectBinary
{
public:
	GenRegularRectBinary(const std::string& json_file, 
		const std::string& img_id_file);
	~GenRegularRectBinary();

	void PackToBinary() const;

private:
	void LoadRegularRectPackFile(const std::string& json_file, 
		const std::string& img_id_file);

private:
	struct Rect
	{
		int w, h;
		int x, y;
	};

	struct Part
	{
		Rect src, dst;
		std::string filepath;
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

		std::string path;
		int id;
	};

private:
	std::string m_filepath;

	std::vector<Picture*> m_pics;

}; // GenRegularRectBinary

}

#endif // _EASYTEXPACKER_GEN_REGULAR_RECT_BINARARY_H_