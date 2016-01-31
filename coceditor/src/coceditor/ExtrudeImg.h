#pragma once

#include <string>


namespace coceditor
{

class ExtrudeImg
{
public:
	ExtrudeImg(const std::string& filename);
	~ExtrudeImg();

	void Trigger(int extrude);

	ee::Rect GetRectTrimed() const {
		ee::Rect ret(_trimed_region);
		ret.translate(ee::Vector(-_width*0.5f, -_height*0.5f));
		return ret;
	}
	std::string GetFileName() const;

private:
	void Load();

	void Trim();
	bool IsTransparent(unsigned char* pixels, int x, int y, int channels);

	void Extrude(int extrude);

private:
	std::string _filename;
	int _width, _height;
	int _format;
	unsigned char* _pixels;

	ee::Rect _trimed_region;
	unsigned char* _trimed;

	int _extrude_size;
	unsigned char* _extruded;

}; // ExtrudeImg

}