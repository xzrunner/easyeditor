#include "LabelNew.h"
//#include "DynamicFont.h"
#include "DynamicTexAndFont.h"

#include "common/color_trans.h"
#include "dataset/text_util.h"
#include "dataset/TPNode.h"
#include "view/Screen.h"
#include "render/ShaderNew.h"

namespace d2d
{

void LabelNew::Print(const Screen& screen, const char* text, const Vector& pos,
					 const LabelStyle& style)
{
	if (strlen(text) == 0) {
		return;
	}

	int sz = strlen(text);
// 	if (sz > 100) {
// 		
// 	}

	const LabelLayout::Layout* layout = LabelLayout::Instance()->Query(text);
	if (!layout) 
	{
		LabelLayout::Layout* new_layout = new LabelLayout::Layout;
		std::vector<int> unicodes;
		std::vector<wxString> utf8s;
		TransToUnicodes(text, unicodes, utf8s);

		std::vector<Line> lines;
		int tot_line_height = TransToLines(unicodes, utf8s, style, lines);

		if (!lines.empty()) {
			DrawLines(screen, pos, style, lines, tot_line_height, *new_layout);
		}

		layout = new_layout;
		LabelLayout::Instance()->Insert(text, layout);
	}
	Draw(layout);
}

void LabelNew::TransToUnicodes(const char* text, std::vector<int>& unicodes, std::vector<wxString>& utf8s)
{
	// 默认已经是utf8的
//	std::string utf8 = string2utf8(text);
	std::string utf8 = text;
	utf8_to_unicode(utf8.c_str(), unicodes, utf8s);
}

int LabelNew::TransToLines(const std::vector<int>& unicodes,
						   const std::vector<wxString>& utf8s,
						   const LabelStyle& style,
						   std::vector<Line>& lines)
{
	assert(unicodes.size() == utf8s.size());

//	DynamicFont* dfont = DynamicFont::Instance();
	DynamicTexAndFont* dfont = DynamicTexAndFont::Instance();

	const int color = trans_color2int(style.color, PT_RGBA);
	int tot_line_height = 0;
	Line line;
	for (int i = 0, n = unicodes.size(); i < n; ++i) 
	{
		int unicode = unicodes[i];
		wxString uft8 = utf8s[i];
		if (unicode == '\n') {
			tot_line_height += line.height;
			lines.push_back(line);
			line.Clear();
		}

		const Glyph* g = dfont->QueryAndInsertFont(unicode, uft8, style.font_size, color, style.has_edge);
		if (g) {
			if (line.width + g->advande > style.width) {
				tot_line_height += line.height;
				lines.push_back(line);
				line.Clear();
			}
			if (line.height == 0) {
				line.height = g->metrics_height;
			}
			line.width += g->advande;
			line.glyphs.push_back(g);
		}
		if (line.height == 0 && g) {
			line.height = g->metrics_height;
		}
	}
	if (!line.glyphs.empty()) {
		tot_line_height += line.height;
		lines.push_back(line);
	}

	return tot_line_height;
}

void LabelNew::DrawLines(const Screen& screen,
						 const Vector& pos,
						 const LabelStyle& style,
						 const std::vector<Line>& lines,
						 int tot_line_height,
						 LabelLayout::Layout& layout)
{
//	DynamicFont* dfont = DynamicFont::Instance();
	DynamicTexAndFont* dfont = DynamicTexAndFont::Instance();

	//Vector vertices[4], texcoords[4];
	float xmin, xmax, ymin, ymax;
	float txmin, txmax, tymin, tymax;
	const float tex_width = dfont->GetWidth(),
		tex_height = dfont->GetHeight();
	const int tex_id = dfont->GetTextureID();
	const int padding = dfont->GetPadding();

	int y = pos.y + style.height*0.5f;
	if (style.align_vert == VAT_TOP) {
		y = pos.y + style.height*0.5f;
	} else if (style.align_vert == VAT_BOTTOM) {
		y = pos.y - style.height*0.5f + tot_line_height;
	} else if (style.align_vert == VAT_CENTER) {
		y = pos.y + tot_line_height * 0.5f;
	}
	for (int i = 0, n = lines.size(); i < n; ++i) {
		// todo discard line which outof label
		const Line& line = lines[i];
		float x = pos.x - style.width*0.5f;
		if (style.align_hori == HAT_LEFT) {
			x = pos.x - style.width*0.5f;
		} else if (style.align_hori == HAT_RIGHT) {
			x = pos.x + style.width*0.5f - line.width;
		} else if (style.align_hori == HAT_CENTER) {
			x = pos.x -line.width * 0.5f;
		}
		for (int j = 0, m = line.glyphs.size(); j < m; ++j) {
			const Glyph* g = line.glyphs[j];
			if (!g) {
				continue;
			}

			const TPNode* r = g->tpnode;

			LabelLayout::Glyph glyph;

			xmin = x + g->bearing_x;
			ymax = y + g->bearing_y - g->metrics_height;
			if (r->IsRotated()) {
				xmax = xmin + (r->GetHeight()-padding*2);
				ymin = ymax - (r->GetWidth()-padding*2);
			} else {
				xmax = xmin + (r->GetWidth()-padding*2);
				ymin = ymax - (r->GetHeight()-padding*2);
			}

			glyph.vertices[0].set(xmin, ymin);
			glyph.vertices[1].set(xmax, ymin);
			glyph.vertices[2].set(xmax, ymax);
			glyph.vertices[3].set(xmin, ymax);
			for (int i = 0; i < 4; ++i) {
				screen.TransPosForRender(glyph.vertices[i]);
			}

			txmin = (r->GetMinX()+padding+0.5f) / tex_width;
			txmax = (r->GetMaxX()-padding-0.5f) / tex_width;
			tymin = (r->GetMinY()+padding+0.5f) / tex_height;
			tymax = (r->GetMaxY()-padding-0.5f) / tex_height;

			if (r->IsRotated())
			{
				d2d::Vector tmp = glyph.vertices[3];
				glyph.vertices[3] = glyph.vertices[2];
				glyph.vertices[2] = glyph.vertices[1];
				glyph.vertices[1] = glyph.vertices[0];
				glyph.vertices[0] = tmp;
			}

			glyph.texcoords[0].set(txmin, tymin);
			glyph.texcoords[1].set(txmax, tymin);
			glyph.texcoords[2].set(txmax, tymax);
			glyph.texcoords[3].set(txmin, tymax);

// 			ShaderNew* shader = ShaderNew::Instance();
// 			shader->sprite();
// 			shader->Draw(vertices, texcoords, tex_id);
			layout.glyphs.push_back(glyph);

			x += g->advande;
		}
		y -= line.height;
	}
}

void LabelNew::Draw(const LabelLayout::Layout* layout)
{
	ShaderNew* shader = ShaderNew::Instance();
	shader->sprite();

	DynamicTexAndFont* dfont = DynamicTexAndFont::Instance();
	const int tex_id = dfont->GetTextureID();

	for (int i = 0, n = layout->glyphs.size(); i < n; ++i) {
		const LabelLayout::Glyph& g = layout->glyphs[i];
		shader->Draw(g.vertices, g.texcoords, tex_id);
	}
}

}