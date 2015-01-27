#include "DistanceFieldFont.h"

#include <stdexcept>
#include <drag2d.h>
#include <freetype/ftglyph.h>

namespace edb
{

DistanceFieldFont::DistanceFieldFont(const char* fontfilepath)
{
	initFreeType(fontfilepath);

	m_distance = new float[32*32];
	m_cache = NULL;
	m_edge = 0;
}

DistanceFieldFont::~DistanceFieldFont()
{
	releaseFreeType();

	delete[] m_distance;
	delete[] m_cache;
}

void DistanceFieldFont::transLuaFileToList(const char* luafilename, const char* outputfile) const
{
	std::set<int> unicodes;

	std::locale::global(std::locale(""));
	std::ifstream fin(luafilename);
	std::locale::global(std::locale("C"));
	std::string str;
	while (getline(fin, str)) {
		transString2Unicode(str.c_str(), unicodes);
	}
	fin.close();

	std::locale::global(std::locale(""));
	std::ofstream fout(outputfile);
	std::locale::global(std::locale("C"));
	std::set<int>::iterator itr = unicodes.begin();
	for ( ; itr != unicodes.end(); ++itr) {
		fout << *itr << " ";
	}
	fout.close();
}

void DistanceFieldFont::readFromLuaFile(const char* filename) const
{
}

void DistanceFieldFont::test(const char* filename)
{
// 	std::ifstream fin(filename);
// 	std::string str;
// 	while (getline(fin, str)) {
// 		std::set<int> unicodes;
// 		transString2Unicode(str.c_str(), unicodes);
// 		for (int i = 0, n = unicodes.size(); i < n; ++i) {
// 			genChar(unicodes[i]);
// 		}
// 	}
// 	fin.close();
}

void DistanceFieldFont::initFreeType(const char* fontfilepath)
{
	//Create and initilize a freetype font library.
	if (FT_Init_FreeType(&m_library)) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face(m_library, fontfilepath, 0, &m_face)) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
//	int h = 4096;
	int h = 3200;
//	int h = 1600;
	FT_Set_Char_Size(m_face, h << 6, h << 6, 96, 96);
}

void DistanceFieldFont::releaseFreeType()
{
	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	FT_Done_Face(m_face);

	//Ditto for the library.
	FT_Done_FreeType(m_library);
}

inline int alpha_index(int x, int y, int width)
{
	return (y*width+x)*4+3;
}

void DistanceFieldFont::genChar(int unicode)
{
	//Load the Glyph for our character.
	if(FT_Load_Glyph( m_face, FT_Get_Char_Index( m_face, unicode ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the m_face's glyph into a Glyph object.
	FT_Glyph glyph;
	if(FT_Get_Glyph( m_face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	// todo 
	if (bitmap.width == 0 || bitmap.rows == 0) {
		return;
	}

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	//Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[4 * width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	int xoffset = (width - bitmap.width) >> 1;
	int yoffset = (height - bitmap.rows) >> 1;
	int ptr = 0;
	for(int j=0; j < height; j++) {
		for(int i=0; i < width; i++){
// 			unsigned char p = (i>=bitmap.width || j>=bitmap.rows) ?
// 				0 : bitmap.buffer[i + bitmap.width*j];

			unsigned char p = (i < xoffset || i >= bitmap.width + xoffset || 
				j < yoffset || j >= bitmap.rows + yoffset) ?
				0 : bitmap.buffer[(i-xoffset) + bitmap.width*(j-yoffset)];

 			if (p != 0) {
 				p = 255;
 			}

			for (int j = 0; j < 3; ++j) {
				expanded_data[ptr++] = 0;
			}
			expanded_data[ptr++] = p;

// 			for (int j = 0; j < 4; ++j) {
// 				expanded_data[ptr++] = p;
// 			}
		}
	}

	unsigned char* extracted = extraceEdge(expanded_data, width, height);

	assert(width == height);
	if (m_edge != width) {
		m_edge = width;

		delete[] m_cache;
		int size = width * height;
		m_cache = new float[size];
		memset(&m_cache[0], 0, sizeof(float) * size);
	}

	unsigned char* result = genDistanceFieldData(expanded_data, extracted, width, height);
	std::string filepath = "e:/text_" + wxString::FromDouble(unicode);
//	d2d::ImageSaver::storeToFile(extracted, width, height, filepath, d2d::ImageSaver::e_png);
	d2d::ImageSaver::storeToFile(result, 32, 32, filepath, d2d::ImageSaver::e_png);

	delete[] expanded_data;
	delete[] extracted;		
	delete[] result;
}

int DistanceFieldFont::copystr(char *utf8, const char *str, int n)  const
{
	int i;
	utf8[0] = str[0];
	int unicode = utf8[0] & ((1 << (8-n)) - 1);
	for (i=1;i<n;i++) {
		utf8[i] = str[i];
		unicode = unicode << 6 | (utf8[i] & 0x3f);
	}
	utf8[i] = 0;
	return unicode;
}

void DistanceFieldFont::transString2Unicode(const char* str, std::set<int>& unicodes) const
{
	char utf8[7];
	for (int i=0; str[i] ;) 
	{
		int unicode;
		uint8_t c = (uint8_t)str[i];
		if ((c&0x80) == 0) {
			unicode = copystr(utf8,&str[i],1);
			i+=1;
		} else if ((c&0xe0) == 0xc0) {
			unicode = copystr(utf8,&str[i],2);
			i+=2;
		} else if ((c&0xf0) == 0xe0) {
			unicode = copystr(utf8,&str[i],3);
			i+=3;
		} else if ((c&0xf8) == 0xf0) {
			unicode = copystr(utf8,&str[i],4);
			i+=4;
		} else if ((c&0xfc) == 0xf8) {
			unicode = copystr(utf8,&str[i],5);
			i+=5;
		} else {
			unicode = copystr(utf8,&str[i],6);
			i+=6;
		}
		unicodes.insert(unicode);
	}
}

unsigned char* DistanceFieldFont::extraceEdge(unsigned char* pixels, int width, int height) const
{
	int size = width * height * 4;
	unsigned char* fixed = new unsigned char[size];
	memcpy(&fixed[0], &pixels[0], size);

	int linesize = 4 * width;
	int ptr = 3;
	for(int j=0; j < height; j++) {
		for(int i=0; i < width; i++){
			bool is_edge = false;
			if (pixels[ptr])
			{
				// left
				if (!is_edge && i > 0 && !pixels[ptr-4]) {
					is_edge = true;
				}
				// right
				if (!is_edge && i < width-1 && !pixels[ptr+4]) {
					is_edge = true;
				}
				// top left
				if (!is_edge && i > 0 && j > 0 && !pixels[ptr-4-linesize]) {
					is_edge = true;
				}
				// top
				if (!is_edge && j > 0 && !pixels[ptr-linesize]) {
					is_edge = true;
				}
				// top right
				if (!is_edge && i < width-1 && j > 0 && !pixels[ptr+4-linesize]) {
					is_edge = true;
				}
				// bottom left
				if (!is_edge && i > 0 && j < height-1 && !pixels[ptr-4+linesize]) {
					is_edge = true;
				}
				// bottom
				if (!is_edge && j < height-1 && !pixels[ptr+linesize]) {
					is_edge = true;
				}
				// bottom right
				if (!is_edge && i < width-1 && j < height-1 && !pixels[ptr+4+linesize]) {
					is_edge = true;
				}
			}
			fixed[ptr] = is_edge ? 255 : 0;
			ptr += 4;			
		}
	}

	return fixed;
}

unsigned char* DistanceFieldFont::genDistanceFieldData(unsigned char* pixels, unsigned char* edge_pixels, int width, int height) const
{
	assert(width == 4096 && height == 4096);
	int pos = 0;
	float min = FLT_MAX, max = -FLT_MAX;
	for (int j = 64; j < height; j += 128) {
		for (int i = 64; i < width; i += 128) {
			float dis = getDistanceToEdge(pixels, edge_pixels, width, height, i, j);
			m_distance[pos++] = dis;
			if (dis < min) {
				min = dis;
			}
			if (dis > max) {
				max = dis;
			}
		}
	}

	unsigned char* scaled = new unsigned char[32 * 32 * 4];
	pos = 0;
	for (int j = 64; j < height; j += 128) {
		for (int i = 64; i < width; i += 128) {
			float dis = m_distance[pos];
			int _pos = (pos << 2);
			for (int i = 0; i < 3; ++i) {
				scaled[_pos++] = 0;
			}
			
			if (dis < 0) {
				scaled[_pos] = (0.5 + 0.5 * (dis / min)) * 255;
			} else if (dis >= 0) {
				scaled[_pos] = (0 + 0.5 * ((max - dis) / max)) * 255;
//				scaled[_pos] = 0;
			}

			++pos;
		}
	}

	return scaled;
}

float DistanceFieldFont::getDistanceToEdge(unsigned char* pixels, unsigned char* edge_pixels, int width, int height, int x, int y) const
{
	memset(&m_cache[0], 0, sizeof(float) * m_edge * m_edge);

	int r = 1;
	while (true)
	{
		int sx = std::max(0, x - r),
			ex = std::min(width-1, x + r);
		int sy = std::max(0, y - r),
			ey = std::min(height-1, y + r);
		int sr = r * r;

		float nearest = FLT_MAX;
		int nx, ny;
		for (int i = sx; i <= ex; ++i) {
			for (int j = sy; j <= ey; ++j) {
				int pos = j * width + i;
				if (m_cache[pos] == -1) {
					continue;
				}

				float s_dis;
				if (m_cache[pos] == 0) {
					if (isEdge(edge_pixels, width, height, i, j)) {
						s_dis = d2d::Math::getDistanceSquare(d2d::Vector(x, y), d2d::Vector(i, j));
						m_cache[pos] = s_dis + 1;
					} else {
						m_cache[pos] = -1;	// not edge
						continue;
					}
				} else if (m_cache[pos] > 0) {
					s_dis = m_cache[pos] - 1;
				}

				if (s_dis > sr) {
					continue;
				}

				if (s_dis < nearest) {
					nearest = s_dis;
					nx = i;
					ny = j;
				}
			}
		}

		// 128 = 4096 / 32
		if (nearest != FLT_MAX || r > 128) {
			nearest = sqrt(nearest);
			int index = alpha_index(x, y, width);
			if (pixels[index] > 0) {
				return -nearest;
			} else {
				return nearest;
			}
		} else {
			++r;
		}
	}
}

bool DistanceFieldFont::isEdge(unsigned char* pixels, int width, int height, int x, int y) const
{
	if (x < 0 || x >= width || y < 0 || y > height) {
		return false;
	}

	int index = alpha_index(x, y, width);
	return pixels[index] == 255;
}

}