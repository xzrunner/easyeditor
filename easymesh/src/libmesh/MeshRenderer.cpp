#include "MeshRenderer.h"
#include "Mesh.h"
#include "Triangle.h"
#include "color_config.h"

#include <ee/SpriteRenderer.h>
#include <ee/RenderContextStack.h>
#include <ee/GL.h>
#include <ee/Math2D.h>

#include <shaderlab.h>
#include <dtex_facade.h>

#include <sprite2/S2_RVG.h>

#include <set>
#include <vector>

namespace emesh
{

void MeshRenderer::DrawInfoUV(const Mesh* mesh, const sm::mat4* mt)
{
	std::set<sm::vec2, sm::Vector2Cmp> unique;
	std::vector<sm::vec2> tmp(3);
	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	float w = mesh->GetWidth(),
		  h = mesh->GetHeight();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * w;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * h;
			if (mt) {
				tmp[i] = *mt * tmp[i];
			}
			unique.insert(tmp[i]);
		}
		s2::RVG::SetColor(RED);
		s2::RVG::Polyline(tmp, true);
	}
	std::vector<sm::vec2> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	s2::RVG::SetColor(BLUE);
	s2::RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void MeshRenderer::DrawInfoXY(const Mesh* mesh, const sm::mat4* mt)
{
	std::set<sm::vec2, sm::Vector2Cmp> unique;
	std::vector<sm::vec2> tmp(3);
	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			if (mt) {
				tmp[i] = *mt * tmp[i];
			}
			unique.insert(tmp[i]);
		}
		s2::RVG::SetColor(RED);
		s2::RVG::Polyline(tmp, true);
	}
	std::vector<sm::vec2> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	s2::RVG::SetColor(BLUE);
	s2::RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void MeshRenderer::DrawTexture(const Mesh* mesh, const s2::RenderParams& params,
							   const ee::Symbol* base_sym)
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	ee::RenderContextStack* rc = ee::RenderContextStack::Instance();

	sm::vec2 ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(sm::vec2(0, 0), 1);
	int edge = dtexf_t0_get_texture_size();
	rc->SetProjection(edge, edge);
	ee::GL::Viewport(0, 0, edge, edge);

	DrawMeshToTmp(mesh, params, base_sym);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	ee::GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(mesh, params);
}

void MeshRenderer::DrawMesh(const Mesh* mesh, const s2::RenderParams& params, int texid)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
	shader->SetColorMap(params.color.rmap.ToABGR(), params.color.gmap.ToABGR(), params.color.bmap.ToABGR());

	int dst_edge = dtexf_t0_get_texture_size();
	float ori_w = mesh->GetWidth(),
		  ori_h = mesh->GetHeight();

	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* tri = tris[i];
		sm::vec2 vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = ee::Math2D::TransVector(tri->nodes[i]->xy, params.mt);
			texcoords[i].x = (tri->nodes[i]->uv.x * ori_w - ori_w * 0.5f + dst_edge * 0.5f) / dst_edge;
			texcoords[i].y = (tri->nodes[i]->uv.y * ori_h - ori_h * 0.5f + dst_edge * 0.5f) / dst_edge;
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
	}
}

void MeshRenderer::DrawMeshToTmp(const Mesh* mesh, const s2::RenderParams& params,
								 const ee::Symbol* base_sym)
{
	dtexf_t0_bind();
	dtexf_t0_clear(0, -2, 2, 0);

	s2::RenderParams _params = params;
	_params.mt.Identity();
	if (base_sym) {
		ee::SpriteRenderer::Draw(base_sym, _params);
	} else {
		ee::SpriteRenderer::Draw(mesh->GetBaseSymbol(), _params);
	}

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	dtexf_t0_unbind();
}

void MeshRenderer::DrawTmpToScreen(const Mesh* mesh, const s2::RenderParams& params)
{
	DrawMesh(mesh, params, dtexf_t0_get_texture_id());
}

}