#include "Font.h"

//OpenGL Headers 
#include <windows.h>		//(the GL headers need it)
#include <GL/gl.h>

#include <freetype/ftglyph.h>
#include <wx/filename.h>

#include "common/Exception.h"

namespace d2d
{

Font::Font(bool stroke /*= false*/)
	: m_stroke(stroke)
{
	m_face = NULL;

	h = DEFAULT_SIZE;
	textures = NULL;
	list_base = 0;
}

Font::~Font()
{
	FontMgr::Instance()->removeItem(m_filename);

	glDeleteLists(list_base,128);
	glDeleteTextures(128,textures);
	delete [] textures;

	if (m_face) {
		FT_Done_Face(m_face);
	}
}

bool Font::loadFromFile(const wxString& filepath)
{
	m_filename = filepath;

	//Allocate some memory to store the texture ids.
	textures = new GLuint[128];

	this->h=h;

	if (filepath.Contains("default")) 
	{
		list_base=glGenLists(128);
		glGenTextures( 128, textures );

		for(unsigned char i=0;i<128;i++)
			make_dlist_wx(i);
	} 
	else 
	{
		if (!wxFileName::FileExists(filepath)) {
			throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
		}

		//Create and initilize a freetype font library.
		FT_Library library;
		if (FT_Init_FreeType( &library )) 
			throw std::runtime_error("FT_Init_FreeType failed");

		//This is where we load in the font information from the file.
		//Of all the places where the code might die, this is the most likely,
		//as FT_New_Face will die if the font file does not exist or is somehow broken.
		if (FT_New_Face( library, filepath.c_str(), 0, &m_face )) 
			throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

		//For some twisted reason, Freetype measures font size
		//in terms of 1/64ths of pixels.  Thus, to make a font
		//h pixels high, we need to request a size of h*64.
		//(h << 6 is just a prettier way of writting h*64)
		FT_Set_Char_Size( m_face, h << 6, h << 6, 96, 96);

		//Here we ask opengl to allocate resources for
		//all the textures and displays lists which we
		//are about to create.  
		list_base=glGenLists(128);
		glGenTextures( 128, textures );

		//This is where we actually create each of the fonts display lists.
		for(unsigned char i=0;i<128;i++)
			make_dlist_freetypes(i);

		//We don't need the face information now that the display
		//lists have been created, so we free the assosiated resources.
		FT_Done_Face(m_face);

		//Ditto for the library.
		FT_Done_FreeType(library);
	}

	return true;
}

// void Font::setSize(unsigned int h)
// {
// 	this->h=h;
// 
// 	FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);
// }

///This function gets the first power of 2 >= the
///int that we pass it.
inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

void Font::make_dlist_freetypes(char ch)
{
	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
	if(FT_Load_Glyph( m_face, FT_Get_Char_Index( m_face, ch ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
	FT_Glyph glyph;
	if(FT_Get_Glyph( m_face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	//Allocate memory for the texture data.
	
	GLubyte* expanded_data = m_stroke ? new GLubyte[width * height * 2] : new GLubyte[width * height];
//	memset(&expanded_data[0], 0, width * height);

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			int c = (i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
			if (m_stroke) {
				expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = c;
			} else {
				expanded_data[i+j*width] = c;
			}
		}
	}


	//Now we just setup some texture paramaters.
	glBindTexture( GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, /*GL_LUMINANCE_ALPHA*/GL_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

	//With the texture created, we don't need to expanded data anymore
	delete [] expanded_data;

	//So now we can create the display list
	glNewList(list_base+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,textures[ch]);

	glPushMatrix();

	//first we need to move over a little so that
	//the character has the right amount of space
	//between it and the one before it.
	glTranslatef(bitmap_glyph->left,0,0);

	//Now we move down a little in the case that the
	//bitmap extends past the bottom of the line 
	//(this is only true for characters like 'g' or 'y'.
	glTranslatef(0,bitmap_glyph->top-bitmap.rows,0);

	//Now we need to account for the fact that many of
	//our textures are filled with empty padding space.
	//We figure what portion of the texture is used by 
	//the actual character and store that information in 
	//the x and y variables, then when we draw the
	//quad, we will only reference the parts of the texture
	//that we contain the character itself.
	float	x=(float)bitmap.width / (float)width,
		y=(float)bitmap.rows / (float)height;

	//Here we draw the texturemaped quads.
	//The bitmap that we got from FreeType was not 
	//oriented quite like we would like it to be,
	//so we need to link the texture to the quad
	//so that the result will be properly aligned.
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
	glTexCoord2d(0,y); glVertex2f(0,0);
	glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
	glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(m_face->glyph->advance.x >> 6 ,0,0);


	//increment the raster position as if we were a bitmap font.
	//(only needed if you want to calculate text length)
	//glBitmap(0,0,0,0,m_face->glyph->advance.x >> 6,0,NULL);

	//Finnish the display list
	glEndList();
}

void Font::make_dlist_wx(char ch)
{
	wxMemoryDC dc;

	dc.SetPen(wxPen(wxColour(255, 255, 255), 3));

	wxSize size = dc.GetTextExtent(ch);

	wxBitmap bmp(size);
	dc.SelectObject(bmp);
	dc.SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxNORMAL));

	dc.DrawRectangle(0, 0, 100, 100);
	dc.DrawText(ch, 0, 0);

	wxImage img = bmp.ConvertToImage();
	unsigned char* src_data = img.GetData();

	int w = size.GetWidth();
	int h = size.GetHeight();
	int w_exp = next_p2(w);
	int h_exp = next_p2(h);

	GLubyte* expanded_data = new GLubyte[w_exp * h_exp * 4];

	memset(&expanded_data[0], 0, w_exp * h_exp * 4);
	for(int j=0; j <h_exp;j++) {
		for(int i=0; i < w_exp; i++) {
			if (j < h && i < w) {
				for (int k = 0; k < 4; ++k) {
					expanded_data[(i+j*w_exp)*4+k] = src_data[(i+j*w)*3+k];
				}
				expanded_data[(i+j*w_exp)*4+3] = 255;
			}
		}
	}

	glBindTexture( GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w_exp, h_exp,
		0, GL_RGBA, GL_UNSIGNED_BYTE, expanded_data );

	delete [] expanded_data;

	glNewList(list_base+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,textures[ch]);

	glPushMatrix();

 	float x=(float)(w+0.5f) / (float)w_exp,
 		y=(float)(h+0.5f) / (float)h_exp;

	glBegin(GL_QUADS);
		glTexCoord2d(0,0); glVertex2f(0,h);
		glTexCoord2d(0,y); glVertex2f(0,0);
		glTexCoord2d(x,y); glVertex2f(w,0);
		glTexCoord2d(x,0); glVertex2f(w,h);
	glEnd();

	glPopMatrix();
	glTranslatef(w,0,0);

	glEndList();
}

} // d2d