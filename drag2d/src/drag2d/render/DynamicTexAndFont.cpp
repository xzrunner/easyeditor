#include "DynamicTexAndFont.h"

#include "common/FileNameTools.h"
#include "common/Config.h"
#include "dataset/TPNode.h"
#include "dataset/ISymbol.h"
#include "render/ShaderMgr.h"
#include "render/SpriteRenderer.h"
#include "render/RenderContext.h"

#include <gl/GL.h>
#include <algorithm>

namespace d2d
{

const int DynamicTexAndFont::WIDTH = 4096;
const int DynamicTexAndFont::HEIGHT = 4096;
const int DynamicTexAndFont::PADDING = 0;
const int DynamicTexAndFont::EXTRUDE = 1;

DynamicTexAndFont* DynamicTexAndFont::m_instance = NULL;
DynamicTexAndFont* DynamicTexAndFont::Instance()
{
	if (!m_instance) {
		m_instance = new DynamicTexAndFont();

		const std::vector<std::pair<std::string, std::string> >& 
			fonts = Config::Instance()->GetFonts();
		if (!fonts.empty() && FilenameTools::IsFileExist(fonts[0].second)) {
			m_instance->LoadFontFile(fonts[0].second.c_str());
		}
	}
	return m_instance;
}

DynamicTexAndFont::DynamicTexAndFont()
	: DynamicPacker(WIDTH, HEIGHT, PADDING, EXTRUDE)
	, m_preload_idx(0)
{
}

void DynamicTexAndFont::BeginImage()
{
	++m_preload_idx;
}

void DynamicTexAndFont::AddImage(Image* img)
{
	// find
	const std::string& filepath = img->GetFilepath();
	if (m_path2node.find(filepath) != m_path2node.end()) {
		return;
	}

	// todo need insert directly
	if (m_preload_idx != 0) {
		img->Retain();
		m_preload_list.push_back(img);
	} else {
		BeginDraw();
		InsertImage(img);
		EndDraw();
	}
}

void DynamicTexAndFont::EndImage()
{
	--m_preload_idx;

	if (m_preload_idx > 0 || m_preload_list.empty()) {
		return;
	}

	BeginDraw();

	std::sort(m_preload_list.begin(), m_preload_list.end(), ImageSizeCmp());
	std::vector<Image*>::iterator itr = m_preload_list.begin();
	for ( ; itr != m_preload_list.end(); ++itr) {
		Image* img = *itr;
		InsertImage(img);
		img->Release();
	}
	m_preload_list.clear();

	EndDraw();
}

void DynamicTexAndFont::AddImageWithRegion(Image* img, const Rect& r_src, const Rect& r_dst, bool rot)
{
	m_rect_preload_list.push_back(ImageWithRegion(img, r_src, r_dst, rot));
}

void DynamicTexAndFont::EndImageWithRegion()
{
	if (m_rect_preload_list.empty()) {
		return;
	}

	// find size of image
	Rect region;
	for (int i = 0, n = m_rect_preload_list.size(); i < n; ++i) {
		region.combine(m_rect_preload_list[i].r_src);
	}

	// insert
	int w = region.xLength();
	int h = region.yLength();
	int extend = GetExtend() * 2;
	d2d::TPNode* node = m_root->Insert(w+extend, h+extend);
	if (!node) {
		m_rect_preload_list.clear();
		return;
	}

	// draw
	BeginDraw();
	for (int i = 0, n = m_rect_preload_list.size(); i < n; ++i) 
	{
		const ImageWithRegion& ir = m_rect_preload_list[i];
		Rect r_vertex, r_texcoords;
		// src
		int ori_width = ir.img->GetOriginWidth(),
			ori_height = ir.img->GetOriginHeight();
		r_texcoords.xMin = ir.r_dst.xMin / ori_width;
		r_texcoords.xMax = ir.r_dst.xMax / ori_width;
		r_texcoords.yMin = (ori_height - ir.r_dst.yMax) / ori_height;
		r_texcoords.yMax = (ori_height - ir.r_dst.yMin) / ori_height;
		// dst
		int extend = GetExtend();
		r_vertex.xMin = node->GetMinX() + extend + ir.r_src.xMin;
		r_vertex.xMax = node->GetMinX() + extend + ir.r_src.xMax;
		r_vertex.yMin = node->GetMinY() + extend + ir.r_src.yMax;
		r_vertex.yMax = node->GetMinY() + extend + ir.r_src.yMin;
		r_vertex.translate(Vector(-m_width*0.5f, -m_height*0.5f));

		bool rot = node->IsRotated() && !ir.rot 
			|| !node->IsRotated() && ir.rot;
		DrawRegion(r_vertex, r_texcoords, ir.img->GetTexID(), rot);
	}
	EndDraw();

	// set key
//	m_path2node.insert(std::make_pair(filepath, n));

	m_rect_preload_list.clear();
}

void DynamicTexAndFont::InsertSymbol(const ISymbol& symbol)
{
	const std::string& filepath = symbol.GetFilepath();
	if (m_path2node.find(filepath) != m_path2node.end()) {
		return;
	}

	Rect r = symbol.GetSize();
	int w = r.xLength();
	int h = r.yLength();
	int extend = GetExtend() * 2;
	TPNode* n = m_root->Insert(w+extend, h+extend);
	if (!n) {
		return;
	}

	RefreshSymbol(symbol, *n);

	m_path2node.insert(std::make_pair(filepath, n));
}

void DynamicTexAndFont::RefreshSymbol(const ISymbol& symbol, const TPNode& node)
{
	BeginDraw();

	glScissor(node.GetMinX(), node.GetMinY(), node.GetMaxX() - node.GetMinX(), node.GetMaxY() - node.GetMinY());
	glEnable(GL_SCISSOR_TEST);

 	glClearColor(0, 0, 0, 0);
 	glClear(GL_COLOR_BUFFER_BIT);
 
	// todo why?
//   	glEnable(GL_BLEND);
//   	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Vector pos(node.GetCenterX(), node.GetCenterY());
	float angle = node.IsRotated() ? PI * 0.5f : 0;
	Matrix mt;
	float dx = -m_width*0.5f - symbol.GetSize().xCenter();
	float dy = -m_height*0.5f - symbol.GetSize().yCenter();
	mt.translate(dx, dy);
	SpriteRenderer::Instance()->Draw(&symbol, mt, pos, angle);

	glDisable(GL_SCISSOR_TEST);

	EndDraw();
}

void DynamicTexAndFont::Remove(const std::string& filepath)
{
	std::map<std::string, TPNode*>::const_iterator itr 
		= m_path2node.find(filepath);
	if (itr != m_path2node.end()) {
		itr->second->Clear();
		m_path2node.erase(itr);
	}
}

const TPNode* DynamicTexAndFont::Query(const std::string& filepath) const
{
	std::map<std::string, TPNode*>::const_iterator itr 
		= m_path2node.find(filepath);
	if (itr != m_path2node.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

const Glyph* DynamicTexAndFont::QueryAndInsertFont(int character, const wxString& uft8,
												   int font_size, int color, int is_edge)
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
	if (!buffer) {
		buffer = GenWXChar(uft8, font_size, color, is_edge, layout);
		if (!buffer) {
			return NULL;
		}
	}
	int w = layout.sizer.width;
	int h = layout.sizer.height;
	int extend = GetExtend() * 2;
	TPNode* n = m_root->Insert(w+extend, h+extend);
	if (!n) {
		Clear();
		n = m_root->Insert(w+extend, h+extend);
	}

	if (n) {
		int extend = GetExtend();
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
			glTexSubImage2D(GL_TEXTURE_2D, 0, n->GetMinX()+extend, n->GetMinY()+extend, h, w, GL_RGBA, GL_UNSIGNED_BYTE, rotated);
			glyph->buffer = rotated;
			delete[] buffer;
		} else {
			glTexSubImage2D(GL_TEXTURE_2D, 0, n->GetMinX()+extend, n->GetMinY()+extend, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glyph->buffer = buffer;
		}

		// todo
		//glBindTexture(ShaderMgr::Instance()->GetTexID());

		glyph->is_used = true;
		glyph->bearing_x = layout.bearingX;
		glyph->bearing_y = layout.bearingY;
		glyph->advande = layout.advance;
		glyph->metrics_height = layout.metrics_height;
		glyph->width = w;
		glyph->height = h;
		glyph->tpnode = n;

		return glyph;
	}	

	return NULL;
}

void DynamicTexAndFont::LoadFontFile(const char* filename)
{
	m_ft_render.LoadFont(filename);	
}

void DynamicTexAndFont::Draw(Vector vertices[4], Vector texcoords[4], 
							 const std::string& tex_filepath, int tex_id) const
{
	const TPNode* n = NULL;
	if (Config::Instance()->IsUseDTex()) {
		n = Query(tex_filepath);
	}
	int tid;
	if (n)
	{
		tid = m_tex;

		float extend = GetExtend();
		int t_width = n->GetMaxX() - n->GetMinX() - extend * 2;
		int t_height = n->GetMaxY() - n->GetMinY() - extend * 2;
		for (int i = 0; i < 4; ++i) {
			texcoords[i].x = ((n->GetMinX()+extend) + t_width * texcoords[i].x) / m_width;
			texcoords[i].y = ((n->GetMinY()+extend) + t_height * texcoords[i].y) / m_height;
		}
		if (n->IsRotated())
		{
			d2d::Vector tmp = vertices[3];
			vertices[3] = vertices[2];
			vertices[2] = vertices[1];
			vertices[1] = vertices[0];
			vertices[0] = tmp;
		}
	}
	else
	{
		tid = tex_id;
	}
	d2d::ShaderMgr::Instance()->Draw(vertices, texcoords, tid);
}

void DynamicTexAndFont::Clear()
{
	delete m_root;
	InitRoot();
	m_hash.Clear();

	m_path2node.clear();
	ClearTexture();
}

void DynamicTexAndFont::ReloadPixels()
{
	BeginDraw();

	std::map<std::string, TPNode*>::iterator itr = m_path2node.begin();
	for ( ; itr != m_path2node.end(); ++itr)
	{
		Image* img = ImageMgr::Instance()->GetItem(itr->first);
		assert(img);
		TPNode* n = itr->second;
		assert(n);
		DrawNode(n, img);
	}

	m_hash.Traverse(ReloadTextureVisitor(m_tex, GetExtend()));

	EndDraw();
}

void DynamicTexAndFont::InsertImage(const Image* img)
{
	const std::string& filepath = img->GetFilepath();
	if (m_path2node.find(filepath) != m_path2node.end()) {
		return;
	}

	int w = img->GetClippedWidth();
	int h = img->GetClippedHeight();
	int extend = GetExtend() * 2;
	d2d::TPNode* n = m_root->Insert(w+extend, h+extend);
	if (!n) {
		return;
	}

	DrawNode(n, img);

	m_path2node.insert(std::make_pair(filepath, n));
}

void DynamicTexAndFont::DrawNode(const TPNode* n, const Image* img) const
{
	Rect r_vertex, r_texcoords;

	int extend = GetExtend();
 	r_vertex.xMin = n->GetMinX() + extend;
 	r_vertex.xMax = n->GetMaxX() - extend;
 	r_vertex.yMin = n->GetMinY() + extend;
 	r_vertex.yMax = n->GetMaxY() - extend;
	r_vertex.translate(Vector(-m_width*0.5f, -m_height*0.5f));

	r_texcoords.xMin = r_texcoords.yMin = 0;
	r_texcoords.xMax = r_texcoords.yMax = 1;

	DrawRegion(r_vertex, r_texcoords, img->GetTexID(), n->IsRotated());

	if (m_extrude > 0) {
		DrawExtrude(img, n);
	}
}

void DynamicTexAndFont::DrawRegion(const Rect& vertex, const Rect& texcoords, int texid, bool is_rotate) const
{
	float vb[16];
	vb[0] = vertex.xMin;
	vb[1] = vertex.yMin;
	vb[4] = vertex.xMax;
	vb[5] = vertex.yMin;
	vb[8] = vertex.xMax;
	vb[9] = vertex.yMax;
	vb[12] = vertex.xMin;
	vb[13] = vertex.yMax;

 	if (is_rotate)
 	{
 		vb[2] = texcoords.xMin;
 		vb[3] = texcoords.yMax;
 		vb[6] = texcoords.xMin;
 		vb[7] = texcoords.yMin;
 		vb[10] = texcoords.xMax;
 		vb[11] = texcoords.yMin;
 		vb[14] = texcoords.xMax;
 		vb[15] = texcoords.yMax;
 	}
 	else
	{
		vb[2] = texcoords.xMin;
		vb[3] = texcoords.yMin;
		vb[6] = texcoords.xMax;
		vb[7] = texcoords.yMin;
		vb[10] = texcoords.xMax;
		vb[11] = texcoords.yMax;
		vb[14] = texcoords.xMin;
		vb[15] = texcoords.yMax;
	}

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetSpriteColor(Colorf(1, 1, 1, 1), Colorf(0, 0, 0, 0));
	shader->SetSpriteColorTrans(Colorf(1, 0, 0, 0), Colorf(0, 1, 0, 0), Colorf(0, 0, 1, 0));	
	shader->Draw(vb, texid);
}

void DynamicTexAndFont::DrawExtrude(const Image* img, const TPNode* n) const
{
	int ori_width = img->GetOriginWidth(),
		ori_height = img->GetOriginHeight();
	Rect r_vertex, r_texcoords;

	d2d::Rect rv, rt;
	rv.xMin = n->GetMinX() - m_width * 0.5f;
	rv.xMax = n->GetMaxX() - m_width * 0.5f;
	rv.yMin = n->GetMinY() - m_height * 0.5f;
	rv.yMax = n->GetMaxY() - m_height * 0.5f;
	rt.xMin = rt.yMin = 0;
//	rt.xMax = rt.yMax = 1;
	rt.xMax = ori_width;
	rt.yMax = ori_height;
	float extend = GetExtend();
	if (n->IsRotated())
	{
		// up
		r_texcoords.xMin = r_texcoords.xMax = (rt.xMax - 0.5f) / ori_width;
		r_texcoords.yMin = rt.yMin / ori_height;
		r_texcoords.yMax = rt.yMax / ori_height;
		r_vertex.xMin = rv.xMin+extend;
		r_vertex.xMax = rv.xMax-extend;
		r_vertex.yMin = rv.yMax-extend;
		r_vertex.yMax = rv.yMax-m_padding;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);

		// down
		r_texcoords.xMin = r_texcoords.xMax = (rt.xMin + 0.5f) / ori_width;
		r_vertex.yMin = rv.yMin+m_padding;
		r_vertex.yMax = rv.yMin+extend;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);

		// left
		r_texcoords.xMin = rt.xMin / ori_width;
		r_texcoords.xMax = rt.xMax / ori_width;
		r_texcoords.yMin = r_texcoords.yMax = (rt.yMax - 0.5f) / ori_height;
		r_vertex.yMin = rv.yMin+extend;
		r_vertex.yMax = rv.yMax-extend;
		r_vertex.xMin = rv.xMin+m_padding;
		r_vertex.xMax = rv.xMin+extend;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);

		// right
		r_texcoords.yMin = r_texcoords.yMax = (rt.yMin + 0.5f) / ori_height;
		r_vertex.xMin = rv.xMax-extend;
		r_vertex.xMax = rv.xMax-m_padding;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), true);

 		// up-left
 		r_texcoords.xMin = r_texcoords.xMax = rt.xMin / ori_width;
 		r_texcoords.yMin = r_texcoords.yMax = rt.yMax / ori_height;
 		r_vertex.xMin = rv.xMin+m_padding;
 		r_vertex.xMax = rv.xMin+extend;
 		r_vertex.yMin = rv.yMin+m_padding;
 		r_vertex.yMax = rv.yMin+extend;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
 
 		// up-right
 		r_texcoords.xMin = r_texcoords.xMax = (rt.xMax - 0.5f) / ori_width;
 		r_vertex.yMin = rv.yMax-extend;
 		r_vertex.yMax = rv.yMax-m_padding;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
 
 		// down-right
 		r_texcoords.yMin = r_texcoords.yMax = (rt.yMin + 0.5f) / ori_height;
 		r_vertex.xMin = rv.xMax-extend;
 		r_vertex.xMax = rv.xMax-m_padding;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
 
 		// down-left
 		r_texcoords.xMin = r_texcoords.xMax = (rt.xMin + 0.5f) / ori_width;
 		r_vertex.yMin = rv.yMin+m_padding;
 		r_vertex.yMax = rv.yMin+extend;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
	}
	else
	{
		// up
		r_texcoords.xMin = rt.xMin / ori_width;
		r_texcoords.xMax = rt.xMax / ori_width;
		r_texcoords.yMin = r_texcoords.yMax = (rt.yMax - 0.5f) / ori_height;
		r_vertex.xMin = rv.xMin+extend;
		r_vertex.xMax = rv.xMax-extend;
		r_vertex.yMin = rv.yMax-extend;
		r_vertex.yMax = rv.yMax-m_padding;

		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// down
		r_texcoords.yMin = r_texcoords.yMax = (rt.yMin + 0.5f) / ori_height;
		r_vertex.yMin = rv.yMin+m_padding;
		r_vertex.yMax = rv.yMin+extend;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// left
		r_texcoords.xMin = r_texcoords.xMax = (rt.xMin + 0.5f) / ori_width;
		r_texcoords.yMin = rt.yMin / ori_height;
		r_texcoords.yMax = rt.yMax / ori_height;
		r_vertex.yMin = rv.yMin+extend;
		r_vertex.yMax = rv.yMax-extend;
		r_vertex.xMin = rv.xMin+m_padding;
		r_vertex.xMax = rv.xMin+extend;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

		// right
		r_texcoords.xMin = r_texcoords.xMax = (rt.xMax - 0.5f) / ori_width;
		r_vertex.xMin = rv.xMax-extend;
		r_vertex.xMax = rv.xMax-m_padding;
		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);

 		// up-left
 		r_texcoords.xMin = r_texcoords.xMax = rt.xMin / ori_width;
 		r_texcoords.yMin = r_texcoords.yMax = rt.yMax / ori_height;
 		r_vertex.xMin = rv.xMin+m_padding;
 		r_vertex.xMax = rv.xMin+extend;
 		r_vertex.yMin = rv.yMax-extend;
 		r_vertex.yMax = rv.yMax-m_padding;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
 
 		// up-right
 		r_texcoords.xMin = r_texcoords.xMax = (rt.xMax - 0.5f) / ori_width;
 		r_vertex.xMin = rv.xMax-extend;
 		r_vertex.xMax = rv.xMax-m_padding;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
 
 		// down-right
 		r_texcoords.yMin = r_texcoords.yMax = (rt.yMin + 0.5f) / ori_height;
 		r_vertex.yMin = rv.yMin+m_padding;
 		r_vertex.yMax = rv.yMin+extend;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
 
 		// down-left
 		r_texcoords.xMin = r_texcoords.xMax = (rt.xMin + 0.5f) / ori_width;
 		r_vertex.xMin = rv.xMin+m_padding;
 		r_vertex.xMax = rv.xMin+extend;
 		DrawRegion(r_vertex, r_texcoords, img->GetTexID(), false);
	}
}

uint32_t* DynamicTexAndFont::GenFTChar(int unicode, int font_size, int color, int is_edge, GlyphLayout& layout)
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

uint32_t* DynamicTexAndFont::GenWXChar(const wxString& uft8, int font_size, int color, int is_edge, GlyphLayout& layout)
{
	wxMemoryDC dc;

	dc.SetPen(wxPen(wxColour(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff), 1));
	dc.SetFont(wxFont(font_size, wxDEFAULT, wxNORMAL, wxNORMAL));
	
	wxSize size = dc.GetTextExtent(uft8);
	wxBitmap bmp(size);
	dc.SelectObject(bmp);

 	dc.DrawRectangle(0, 0, 100, 100);
	dc.DrawText(uft8, 0, 0);

	wxImage img = bmp.ConvertToImage();
	unsigned char* src_data = img.GetData();

	layout.advance = size.x;
	layout.bearingX = layout.bearingY = 0;
	layout.metrics_height = size.y;
	layout.sizer.width = size.x;
	layout.sizer.height = size.y;

	int sz = size.x * size.y;
	uint32_t* ret = new uint32_t[sz];
	memset(ret, 0, sizeof(uint32_t) * sz);
	for (int i = 0; i < size.y; ++i) {
		for (int j = 0; j < size.x; ++j) {
			int ptr_src = (size.x * i + j) * 3;
			int ptr_dst = size.x * (size.y - 1 - i) + j;

			uint8_t r = src_data[ptr_src++];
			uint8_t g = src_data[ptr_src++];
			uint8_t b = src_data[ptr_src++];

			uint8_t alpha = 255;
			if (r == 255 && g == 255 && b == 255) {
				alpha = 0;
			}
			uint32_t c = alpha << 24 | b << 16 | g << 8 | r;

			ret[ptr_dst] = c;
		}
	}
	return ret;
}

void DynamicTexAndFont::BeginDraw()
{
	ShaderMgr* shader = ShaderMgr::Instance();

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	m_ori_width = viewport[2];
	m_ori_height = viewport[3];

	RenderContext* context = RenderContext::GetCurrContext();
	context->GetModelView(m_ori_offset, m_ori_scale);

	shader->SetFBO(m_fbo);
	shader->sprite();
	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(m_width, m_height);
	shader->SetSpriteColor(Colorf(1, 1, 1, 1), Colorf(0, 0, 0, 0));
	glViewport(0, 0, m_width, m_height);

   	glEnable(GL_BLEND);
   	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void DynamicTexAndFont::EndDraw()
{
	ShaderMgr* shader = ShaderMgr::Instance();
	// set fbo to force flush
	// todo dtexÁ¬Ðøinsert»áÂý
	shader->SetFBO(0);
	shader->SetModelView(m_ori_offset, m_ori_scale);
	shader->SetProjection(m_ori_width, m_ori_height);
	glViewport(0, 0, m_ori_width, m_ori_height);
}

//////////////////////////////////////////////////////////////////////////
// class DynamicTexAndFont::Hash
//////////////////////////////////////////////////////////////////////////

DynamicTexAndFont::Hash::
Hash()
{
	m_freelist = m_freenode_ptr = NULL;
	memset(m_hash, 0, sizeof(m_hash));

	m_capacity = 10240;
	m_freenode_ptr = m_freelist = new Node[m_capacity];
	memset(m_freelist, 0, sizeof(Node)*m_capacity);
	for (int i = 0; i < m_capacity; i++) {
		Node* n = &m_freelist[i];
		n->next = &m_freelist[i+1];
	}
}

DynamicTexAndFont::Hash::
~Hash()
{
	delete m_freelist;
}

Glyph* DynamicTexAndFont::Hash::
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
	new_node->next = m_hash[h];
	m_hash[h] = new_node;

	new_node->character = character;
	new_node->font_size = font_size;
	new_node->color = color;
	new_node->is_edge = is_edge;

	return &new_node->glyph;
}

void DynamicTexAndFont::Hash::
Clear()
{
	memset(m_hash, 0, sizeof(m_hash));

	m_freenode_ptr = m_freelist;
	memset(m_freelist, 0, sizeof(Node)*m_capacity);
	for (int i = 0; i < m_capacity; i++) {
		Node* n = &m_freelist[i];
		n->next = &m_freelist[i+1];
	}
}

void DynamicTexAndFont::Hash::
Traverse(IVisitor& visitor) const
{
	for (int i = 0; i < HASH_SIZE; ++i)
	{
		Node* n = m_hash[i];
		while (n) {
			bool has_next = true;
			if (n->glyph.is_used) {
				visitor.Visit(&n->glyph, has_next);
			}
			if (!has_next) {
				return;
			}
			n = n->next;
		}
	}
}

int DynamicTexAndFont::Hash::
GetHashVal(int character, int font_size, int color, int is_edge)
{
	if (is_edge != 0 && is_edge != 1) {
		return abs(character ^ font_size ^ color ^ is_edge) % HASH_SIZE;
	} else {
		return (character ^ (font_size * 97) ^ (unsigned int)color ^ (is_edge * 31)) % HASH_SIZE;
	}	
}

//////////////////////////////////////////////////////////////////////////
// class DynamicTexAndFont::ReloadTextureVisitor
//////////////////////////////////////////////////////////////////////////

void DynamicTexAndFont::ReloadTextureVisitor::
Visit(Object* object, bool& bFetchNext)
{
	Glyph* g = static_cast<Glyph*>(object);
	TPNode* n = g->tpnode;

	glBindTexture(GL_TEXTURE_2D, m_tex);
	if (n->IsRotated()) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, n->GetMinX()+m_extend, n->GetMinY()+m_extend, g->height, g->width, GL_RGBA, GL_UNSIGNED_BYTE, g->buffer);
	} else {
		glTexSubImage2D(GL_TEXTURE_2D, 0, n->GetMinX()+m_extend, n->GetMinY()+m_extend, g->width, g->height, GL_RGBA, GL_UNSIGNED_BYTE, g->buffer);
	}

	bFetchNext = true;
}

}