#ifndef _EASYDB_DISTANCE_FIELD_FONT_PARSER_H_
#define _EASYDB_DISTANCE_FIELD_FONT_PARSER_H_

#include <string>

#include <stdint.h>

namespace edb
{

class DFFParser
{
public:
	DFFParser(const char* filepath);

	void outputImage(int width, int height);

	void outputImageFast(int width, int height);

private:
	int computeAlpha(int width, int height, int x, int y);

	int getAlpha(int x, int y);

	void fillingAlphaNew(uint8_t* pixels, int width, int height) const;
	void fillingAlpha(uint8_t* pixels, int width, int height) const;

private:
	static const int EDGE = 32;

private:
	std::string m_dir;

	unsigned char* m_alphas;

}; // DFFParser

}

#endif // _EASYDB_DISTANCE_FIELD_FONT_PARSER_H_
