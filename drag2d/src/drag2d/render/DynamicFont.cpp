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
		m_instance->LoadFontFile("FZCY_GBK.ttf");
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

	m_hash.Init(1024);
}

DynamicFont::~DynamicFont()
{
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffersEXT(1, &m_fbo);

	delete m_root;
}

const Glyph* DynamicFont::LookUp(int character, int font_size, int color, int is_edge)
{
	Glyph* glyph = m_hash.LookUp(character, font_size, color, is_edge);
	if (glyph->is_used) {
		return glyph;
	}
	
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

 	GlyphLayout layout;
 	uint32_t* buffer = GenFTChar(character, font_size, color, is_edge, layout);
 	int w = layout.sizer.width;
 	int h = layout.sizer.height;
 	TPNode* n = m_root->Insert(w, h);
 	if (n) {
		glBindTexture(GL_TEXTURE_2D, m_tex);
		if (n->IsRotated()) {
			uint32_t* rotated = new uint32_t[w*h];
			int ptr_src = 0;
			for (int i = 0; i < h; ++i) {
				for (int j = 0; j < w; ++j) {
					int ptr_dst = j*h + (h-1-i);
					rotated[ptr_dst] = buffer[ptr_src++];
				}
			}
			glTexSubImage2D(GL_TEXTURE_2D, 0, n->GetMinX(), n->GetMinY(), h, w, GL_RGBA, GL_UNSIGNED_BYTE, rotated);
			delete[] rotated;
		} else {
			glTexSubImage2D(GL_TEXTURE_2D, 0, n->GetMinX(), n->GetMinY(), w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
		free(buffer);

		glyph->is_used = true;
		glyph->bearing_x = layout.bearingX;
		glyph->bearing_y = layout.bearingY;
		glyph->advande = layout.advance;
		glyph->tpnode = n;
 	}	

	return NULL;
}

void DynamicFont::LoadFontFile(const char* filename)
{
	m_ft_render.LoadFont(filename);	
}

void DynamicFont::DebugDraw(const Screen& screen) const
{
	DynamicUtils::DebugDraw(screen, m_tex);
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
	memset(m_freelist, 0, sizeof(Node)*capacity);
	for (int i = 0; i < capacity; i++) {
		Node* n = &m_freelist[i];
		n->next = &m_freelist[i+1];
	}
}

Glyph* DynamicFont::Hash::
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
			return &n->glyph;
		}
		n = n->next;
	}

	// insert
	Node* new_node = m_freenode_ptr;
	m_freenode_ptr = m_freenode_ptr->next;
	new_node->next = n;
	m_hash[h] = new_node;

	new_node->character = character;
	new_node->font_size = font_size;
	new_node->color = color;
	new_node->is_edge = is_edge;
	return &new_node->glyph;
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