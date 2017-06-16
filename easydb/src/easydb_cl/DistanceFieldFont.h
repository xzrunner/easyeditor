#ifndef _EASYDB_DISTANCE_FIELD_FONT_H_
#define _EASYDB_DISTANCE_FIELD_FONT_H_

#include <vector>
#include <set>

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>

namespace edb
{

class DistanceFieldFont
{
public:
	DistanceFieldFont(const char* fontfilepath);
	~DistanceFieldFont();
	
	void transLuaFileToList(const char* luafilename, const char* outputfile) const;

	void readFromLuaFile(const char* filename) const;

	void test(const char* filename);

private:
	void initFreeType(const char* fontfilepath);
	void releaseFreeType();

	void genChar(int unicode);
	
	int copystr(char *utf8, const char *str, int n) const;
	void transString2Unicode(const char* str, std::set<int>& unicodes) const;

	unsigned char* extraceEdge(unsigned char* pixels, int width, int height) const;

	unsigned char* genDistanceFieldData(unsigned char* pixels, unsigned char* edge_pixels, int width, int height) const;

	float getDistanceToEdge(unsigned char* pixels, unsigned char* edge_pixels, int width, int height, int x, int y) const;

	bool isEdge(unsigned char* pixels, int width, int height, int x, int y) const;

private:
	FT_Library m_library;
	FT_Face m_face;

	float* m_distance;
	float* m_cache;
	int m_edge;

}; // DistanceFieldFont

}

#endif // _EASYDB_DISTANCE_FIELD_FONT_H_