#include "FontSymbol.h"
#include "Font.h"
#include "TextSprite.h"

#include "render/Shader.h"

//OpenGL Headers 
#include <windows.h>
#include <GL/gl.h>

// STD
#include <vector>

namespace d2d
{

FontSymbol::FontSymbol()
	: m_font(NULL)
{
}

FontSymbol::~FontSymbol()
{
}

FontSymbol* FontSymbol::clone() const
{
	return NULL;
}

void FontSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void FontSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void FontSymbol::reloadTexture() const
{
	
}

void FontSymbol::draw(const Colorf& mul, const Colorf& add,
					  const ISprite* sprite/* = NULL*/) const
{
	TextSprite* text = static_cast<TextSprite*>(const_cast<ISprite*>(sprite));

	const d2d::Colori& c = text->getColor();
	d2d::Colorf _mul(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
	_mul = cMul(_mul, mul);
	Shader::Instance()->color(_mul, add);

	print(0, 0, text->getText().c_str());
}

Rect FontSymbol::getSize(const ISprite* sprite/* = NULL*/) const
{
	int w = 0;
	TextSprite* text = static_cast<TextSprite*>(const_cast<ISprite*>(sprite));
	if (text)
		w = m_font->h * 0.5f * text->getText().size();
	return Rect(w, m_font->h);
}

void FontSymbol::loadResources()
{
	m_font = FontMgr::Instance()->getItem(m_filepath);
}

void FontSymbol::print(float x, float y, const char* text) const
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