#include "LabelNew.h"
#include "DynamicFont.h"

#include "common/color_trans.h"
#include "dataset/text_util.h"
#include "dataset/TPNode.h"
#include "view/Screen.h"
#include "render/ShaderNew.h"

namespace d2d
{

void LabelNew::Print(const Screen& screen, const char* text, const Vector& pos,
					 const LabelStyle& style) const
{
	const int color = trans_color2int(style.color, PT_RGBA);

	DynamicFont* dfont = DynamicFont::Instance();
	std::string utf8 = string2utf8(text);
	std::vector<int> unicodes;
	utf8_to_unicode(utf8.c_str(), unicodes);

	float x = pos.x - style.width*0.5f,
		  y = pos.y + style.height*0.5f;
	const float start_x = x, start_y = y;
	Vector vertices[4], texcoords[4];
	float xmin, xmax, ymin, ymax;
	float txmin, txmax, tymin, tymax;
	const float tex_width = dfont->GetWidth(),
		tex_height = dfont->GetHeight();
	const int tex_id = dfont->GetTextureID();
	const int padding = dfont->GetPadding();
	for (int i = 0, n = unicodes.size(); i < n; ++i) 
	{
		const Glyph* g = dfont->LookUp(unicodes[i], style.font_size, color, style.has_edge);
		if (!g) {
			continue;
		}
		if (x + g->advande - start_x > style.width) {
			x = start_x;
			y -= g->metrics_height;
		}
		if (start_y - (y - g->metrics_height) > style.height) {
			break;
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
}

}