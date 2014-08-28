#include "FontBlankSymbol.h"
#include "FontSprite.h"
#include "Font.h"

#include "common/Vector.h"
#include "common/color_trans.h"
#include "common/FileNameTools.h"
#include "common/FileNameParser.h"
#include "common/Settings.h"
#include "common/Math.h"
#include "dataset/SpriteTools.h"
#include "render/PrimitiveDraw.h"
#include "render/Shader.h"
#include "render/LabelNew.h"

//OpenGL Headers 
#include <windows.h>
#include <GL/gl.h>

#include <json/json.h>

namespace d2d
{

FontBlankSymbol::FontBlankSymbol()
{
	static int id = 0;
	m_name = wxT("fontblank") + wxVariant(id++);

	align_hori = align_vert = 0;
	size = 0;
	width = height = 100;

	m_font = NULL;

	m_style.fill = true;
	m_style.color.set(0.7f, 0.7f, 0.7f, 0.7f);
}

FontBlankSymbol::~FontBlankSymbol()
{
//	m_font->release();
}

void FontBlankSymbol::reloadTexture() const
{
}

void FontBlankSymbol::draw(const Screen& scr,
						   const Matrix& mt,
						   const Colorf& mul, 
						   const Colorf& add,
						   const ISprite* sprite/* = NULL*/) const
{
	if (Settings::DrawFontType & Settings::DrawFontBg) {
		DrawBackground(scr, sprite, mt);
	} 
	if (Settings::DrawFontType & Settings::DrawFontText) {
		DrawText(scr, sprite, mt);
	}

//	SpriteTools::DrawName(scr, sprite, mt);
}

Rect FontBlankSymbol::getSize(const ISprite* sprite/* = NULL*/) const
{
	return Rect(width, height);
}

void FontBlankSymbol::refresh()
{
}

bool FontBlankSymbol::loadFont(const std::string& _filename)
{
	if (_filename == filename)
		return false;

	std::string filename = FilenameTools::getAbsolutePath(m_filepath, _filename);
	if (!FileNameParser::isType(filename, FileNameParser::e_freetype) 
		|| !FilenameTools::isExist(filename))
		return false;

	delete m_font, m_font = NULL;
	m_font = FontMgr::Instance()->getItem(filename);
	return m_font != NULL;
}

void FontBlankSymbol::loadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
	font = value["font"].asString();
	color = value["color"].asString();
	if (!value["align"].isNull()) {
		align_hori = value["align"].asDouble();
	} else {
		align_hori = value["align hori"].asDouble();
		align_vert = value["align vert"].asDouble();
	}
	size = value["size"].asDouble();
	width = value["width"].asDouble();
	height = value["height"].asDouble();
	if (value["font filename"].isNull())
		m_font = NULL;
	else {
		filename = value["font filename"].asString();
		loadFont(filename);
	}
}

void FontBlankSymbol::DrawBackground(const Screen& scr, const ISprite* sprite,
									 const Matrix& mt) const
{
	float w = width, h = height;
	const FontSprite* s = dynamic_cast<const FontSprite*>(sprite);
	if (s)
	{
		w = s->width;
		h = s->height;
		if (m_font)
			glColor4f(s->color.r, s->color.g, s->color.b, s->color.a);
	}
	Vector center(0, 0);
	center = Math::transVector(center, mt);
	PrimitiveDraw::rect(scr, center, w*0.5f, h*0.5f, m_style);
}

void FontBlankSymbol::DrawText(const Screen& scr, const ISprite* sprite, const Matrix& mt) const
{
	if (sprite) {
		if (const FontSprite* font = dynamic_cast<const FontSprite*>(sprite)) {
			const std::string& str = font->GetTextContext();
			if (!str.empty()) {
				LabelNew label;
				label.Print(scr, str.c_str(), sprite->getPosition());
			}
		}
	}

// 	if (sprite) {
// 		if (const FontSprite* font = dynamic_cast<const FontSprite*>(sprite)) {
// 			const std::string& str = font->GetTextContext();
// 			if (!str.empty()) {
// 				d2d::PrimitiveDraw::text(str.c_str());
// 			}
// 		}
// 	}

// 	if (m_font) 
// 	{
// 		float w = width;
// 		if (const FontSprite* s = dynamic_cast<const FontSprite*>(sprite)) {
// 			w = s->width;
// 		}
// 		Shader::Instance()->null();
// 		print(-w*0.5f, 0, "abcdEFGH1234");
// 	}
}

void FontBlankSymbol::print(float x, float y, const char* text) const
{
	// We want a coordinate system where things coresponding to window pixels.
	//	pushScreenCoordinateMatrix();					

	GLuint font=m_font->list_base;
	float h=m_font->h/.63f;						//We make the height about 1.5* that of

	//Here is some code to split the text that we have been
	//given into a set of lines.  
	//This could be made much neater by using
	//a regular expression library such as the one avliable from
	//boost.org (I've only done it out by hand to avoid complicating
	//this tutorial with unnecessary library dependencies).
	const char *start_line=text;
	const char *c;
	std::vector<std::string> lines;
	for(c=text;*c;c++) {
		if(*c=='\n') {
			std::string line;
			for(const char *n=start_line;n<c;n++) line.append(1,*n);
			lines.push_back(line);
			start_line=c+1;
		}
	}
	if(start_line) {
		std::string line;
		for(const char *n=start_line;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	// set in shader
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glListBase(font);

	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.  
	for(int i=0;i<lines.size();i++) {


		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y-h*i,0);
		glMultMatrixf(modelview_matrix);

		//  The commented out raster position stuff can be useful if you need to
		//  know the length of the text that you are creating.
		//  If you decide to use it make sure to also uncomment the glBitmap command
		//  in make_dlist().
		//	glRasterPos2f(0,0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
		//	float rpos[4];
		//	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
		//	float len=x-rpos[0];

		glPopMatrix();
	}

	glPopAttrib();		

	//	pop_projection_matrix();

	glBindTexture(GL_TEXTURE_2D, NULL);
}

} // d2d