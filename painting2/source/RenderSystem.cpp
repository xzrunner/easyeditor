#include "painting2/RenderSystem.h"
#include "painting2/Texture.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace pt2
{

void RenderSystem::DrawTexture(const Texture& tex, const sm::rect& pos,
	                           const sm::Matrix2D& mat)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	
	float vertices[8];
	CalcVertices(pos, mat, vertices);

	float txmin, txmax, tymin, tymax;
	txmin = tymin = 0;
	txmax = tymax = 1;
	float texcoords[8] = {
		txmin, tymin,
		txmax, tymin,
		txmax, tymax,
		txmin, tymax,
	};

	auto shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->DrawQuad(vertices, texcoords, tex.GetTexID());
}

bool RenderSystem::CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat, float* vertices)
{
	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax = -FLT_MAX, ymax = -FLT_MAX;

	const float* mt = mat.x;

	float x, y;

	float* ptr_dst = &vertices[0];

	x = (pos.xmin * mt[0] + pos.ymin * mt[2]) + mt[4];
	y = (pos.xmin * mt[1] + pos.ymin * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (pos.xmax * mt[0] + pos.ymin * mt[2]) + mt[4];
	y = (pos.xmax * mt[1] + pos.ymin * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (pos.xmax * mt[0] + pos.ymax * mt[2]) + mt[4];
	y = (pos.xmax * mt[1] + pos.ymax * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (pos.xmin * mt[0] + pos.ymax * mt[2]) + mt[4];
	y = (pos.xmin * mt[1] + pos.ymax * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	//if (rp.IsViewRegionValid()) {
	//	const sm::rect& vr = rp.GetViewRegion();
	//	if (xmax <= vr.xmin || xmin >= vr.xmax ||
	//		ymax <= vr.ymin || ymin >= vr.ymax) {
	//		return false;
	//	}
	//}

	return true;
}

}