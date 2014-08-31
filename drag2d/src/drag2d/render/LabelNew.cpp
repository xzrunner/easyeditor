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
	std::vector<int> unicodes;
	TransToUnicodes(text, unicodes);

	std::vector<Line> lines;
	int tot_line_height = TransToLines(unicodes, style, lines);

	DrawLines(screen, pos, style, lines, tot_line_height);
}

void LabelNew::TransToUnicodes(const char* text, std::vector<int>& unicodes)
{
//	DynamicFont* dfont = DynamicFont::Instance();
	DynamicTexAndFont* dfont = DynamicTexAndFont::Instance();
	std::string utf8 = string2utf8(text);
	utf8_to_unicode(utf8.c_str(), unicodes);
}

int LabelNew::TransToLines(const std::vector<int>& unicodes, 
						   const LabelStyle& style,
						   std::vector<Line>& lines)
{
//	DynamicFont* dfont = DynamicFont::Instance();
	DynamicTexAndFont* dfont = DynamicTexAndFont::Instance();

	const int color = trans_color2int(style.color, PT_RGBA);
	int tot_line_height = 0;
	Line line;
	for (int i = 0, n = unicodes.size(); i < n; ++i) 
	{
		int unicode = unicodes[i];
		if (unicode == '\n') {
			tot_line_height += line.height;
			lines.push_back(line);
			line.Clear();
		}

		const Glyph* g = dfont->QueryAndInsertFont(unicode, style.font_size, color, style.has_edge);
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
		}
		if (line.height == 0 && g) {
			line.height = g->metrics_height;
		}
		line.glyphs.push_back(g);
	}
	tot_line_height += line.height;
	lines.push_back(line);

	return tot_line_height;
}

void LabelNew::DrawLines(const Screen& screen,
						 const Vector& pos,
						 const LabelStyle& style,
						 const std::vector<Line>& lines,
						 int tot_line_height)
{
//	DynamicFont* dfont = DynamicFont::Instance();
	DynamicTexAndFont* dfont = DynamicTexAndFont::Instance();

	Vector vertices[4], texcoords[4];
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

			xmin = x + g->bearing_x;
			ymax = y + g->bearing_y - g->metrics_height;
			if (r->IsRotated()) {
				xmax = xmin + (r->GetHeight()-padding*2);
				ymin = ymax - (r->GetWidth()-padding*2);
			} else {
				xmax = xmin + (r->GetWidth()-padding*2);
				ymin = ymax - (r->GetHeight()-padding*2);
			}

			vertices[0].set(xmin, ymin);
			vertices[1].set(xmax, ymin);
			vertices[2].set(xmax, ymax);
			vertices[3].set(xmin, ymax);
			for (int i = 0; i < 4; ++i) {
				screen.TransPosForRender(vertices[i]);
			}

			txmin = (r->GetMinX()+padding) / tex_width;
			txmax = (r->GetMaxX()-padding) / tex_width;
			tymin = (r->GetMinY()+padding) / tex_height;
			tymax = (r->GetMaxY()-padding) / tex_height;

			if (r->IsRotated())
			{
				d2d::Vector tmp = vertices[3];
				vertices[3] = vertices[2];
				vertices[2] = vertices[1];
				vertices[1] = vertices[0];
				vertices[0] = tmp;
			}

			texcoords[0].set(txmin, tymin);
			texcoords[1].set(txmax, tymin);
			texcoords[2].set(txmax, tymax);
			texcoords[3].set(txmin, tymax);

			ShaderNew* shader = ShaderNew::Instance();
			shader->sprite();
			shader->Draw(vertices, texcoords, tex_id);

			x += g->advande;
		}
		y -= line.height;
	}
}

}