#ifndef _EASYDB_DISTANCE_FIELD_FONT_PARSER_H_
#define _EASYDB_DISTANCE_FIELD_FONT_PARSER_H_

#include <string>

namespace edb
{

class DFFParser
{
public:
	DFFParser(const char* filepath);

	void outputImage(int width, int height);

private:
	int computeAlpha(int width, int height, int x, int y);

	int getAlpha(int x, int y);

private:
	std::string m_dir;

	unsigned char* m_alphas;

}; // DFFParser

}

#endif // _EASYDB_DISTANCE_FIELD_FONT_PARSER_H_
