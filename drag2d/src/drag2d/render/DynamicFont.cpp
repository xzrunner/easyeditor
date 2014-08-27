#include "DynamicFont.h"
#include "DynamicUtils.h"

#include "dataset/TPNode.h"

#include <opengl/opengl.h>

namespace d2d
{

const int DynamicFont::WIDTH = 512;
const int DynamicFont::HEIGHT = 512;

DynamicFont* DynamicFont::m_instance = NULL;
DynamicFont* DynamicFont::Instance()
{
	if (!m_instance) {
		m_instance = new DynamicFont();
	}
	return m_instance;
}

DynamicFont::DynamicFont()
{
	m_width = WIDTH;
	m_height = HEIGHT;

	initOpenGLExtensions();

	m_tex = DynamicUtils::InitTexture(m_width, m_height);
	m_fbo = DynamicUtils::InitFBO(m_tex);
	m_root = DynamicUtils::InitRoot(m_width, m_height);
}

DynamicFont::~DynamicFont()
{
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffersEXT(1, &m_fbo);

	delete m_root;
}

const Rect* DynamicFont::LookUp(int character, int font_size, int color, int is_edge)
{
	Rect* r = m_hash.LookUp(character, font_size, color, is_edge);
	
// 	if (character == ' ') {
// 		sizer = m_space_sizer[font_size];
// 		if (sizer.width == 0 || sizer.height == 0) {
// 			sizer = m_space_sizer[font_size] = m_ft_render.GetGlyphSizer(unicode, font_size);
// 		}
// 		if (sizer.width == 0 || sizer.height == 0) {
// 			sizer = m_space_sizer[font_size] = m_wx_render.GetGlyphSizer(unicode, font_size);
// 		}
// 		return sizer;
// 	}

// 	GlyphLayout layout;
// 	uint32_t* buffer = GenFTChar(character, font_size, color, is_edge, layout);
// 	int w = layout.sizer.width;
// 	int h = layout.sizer.height;
// 	d2d::TPNode* n = m_root->Insert(img, w, h);
// 	if (!n) {
// 		return;
// 	}	

	return r;
}

void DynamicFont::LoadFontFile(const char* filename)
{
	m_ft_render.LoadFont(filename);	
}

uint32_t* DynamicFont::GenFTChar(int unicode, int font_size, int color, int is_edge, GlyphLayout& layout)
{
	uint32_t* buffer = NULL;
	if (is_edge) 
	{
		union Pixel32 cfont, coutline;
		cfont.Set(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff, color >> 24);
		coutline.Set(0, 0, 0, 255);
		buffer = m_ft_render.WriteGlyphWithStroker(unicode, font_size, cfont, coutline, 2, layout);
	} 
	else 
	{
		union Pixel32 col;
		col.Set(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff, color >> 24);
		buffer = m_ft_render.WriteGlyphNoStroker(unicode, font_size, col, layout);
	}
	return buffer;
}

//////////////////////////////////////////////////////////////////////////
// class DynamicFont::Hash
//////////////////////////////////////////////////////////////////////////

DynamicFont::Hash::
Hash()
{
	m_freelist = m_freenode_ptr = NULL;
	memset(m_hash, 0, sizeof(m_hash));

	// debug
	int zz = sizeof(m_hash);
}

DynamicFont::Hash::
~Hash()
{
	delete m_freelist;
}

void DynamicFont::Hash::
Init(int capacity)
{
	m_freenode_ptr = m_freelist = new Node[capacity];
	memset(m_freelist, 0, sizeof(m_freelist));
}

Rect* DynamicFont::Hash::
LookUp(int character, int font_size, int color, int is_edge)
{
	int h = Hash::GetHashVal(character, font_size, color, is_edge);
	struct Node* n = m_hash[h];
	while (n) {
		if (n->character == character && 
			n->font_size == font_size && 
			n->color == color && 
			n->is_edge == is_edge) 
		{
			return &n->rect;
		}
		n = n->next;
	}

	// insert
	Node* new_node = m_freenode_ptr;
	m_freenode_ptr = m_freenode_ptr->next;
	new_node->next = n;
	m_hash[h] = new_node;

	return &new_node->rect;
}

int DynamicFont::Hash::
GetHashVal(int character, int font_size, int color, int is_edge)
{
	if (is_edge != 0 && is_edge != 1) {
		return abs(character ^ font_size ^ color ^ is_edge) % HASH_SIZE;
	} else {
		return (character ^ (font_size * 97) ^ (unsigned int)color ^ (is_edge * 31)) % HASH_SIZE;
	}	
}

}