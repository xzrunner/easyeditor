#include "OceanMesh.h"
#include "MeshShape.h"
#include "config.h"

#include <ee/Math2D.h>
#include <ee/Triangulation.h>
#include <ee/ImageSymbol.h>

#include <shaderlab.h>

namespace eterrain2d
{

OceanMesh::OceanMesh(const eshape::PolygonShape* shape, 
					 const ee::ImageSymbol* image)
	 : m_shape(shape->Clone())
	 , m_image0(image)
	 , m_image1(NULL)
	 , m_lock_bound(false)
	 , m_during(0)
{
	m_row = MESH_ROW;
	m_col = MESH_COL;

	m_wave_open = true;
	m_wave_speed = WAVE_SPEED * 0.1f;
	m_wave_height = WAVE_HEIGHT;

	m_uv_move_open = true;
	m_texcoords_spd.Set(UV_SPEED_X * 0.01f, UV_SPEED_Y * 0.01f);
	m_texcoords_base.Set(0, 0);

	m_blend_open = true;
	m_blend_base = 0;
	m_blend_speed = BLEND_SPEED * 0.01f;

	Build();
}

OceanMesh::~OceanMesh()
{
	Clear();

	m_shape->Release();
}

void OceanMesh::Build()
{
	Clear();

	std::vector<sm::vec2> bound;
	ee::Math2D::RemoveDuplicatePoints(m_shape->GetVertices(), bound);

	sm::rect r = CalBoundRegion(bound);

	std::vector<sm::vec2> segs;
	CalSegments(r, segs);

	std::vector<sm::vec2> tris_vertices;
	ee::Triangulation::Lines(bound, segs, tris_vertices);
	if (tris_vertices.empty()) {
		return;
	}

	std::vector<sm::vec2> tris_texcoords;
	CalTrisTexcords(r, tris_vertices, tris_texcoords);

	BuildGrids(r, tris_vertices, tris_texcoords, bound);
}

void OceanMesh::SetSmallGridSize(int row, int col)
{
	if (m_row != row || m_col != col) {
		m_row = row;
		m_col = col;
		Build();
	}
}

void OceanMesh::SetWaveInfo(float speed, float height)
{
	m_wave_speed = speed;
	m_wave_height = height;
}

void OceanMesh::SetTexcoordsSpeed(const sm::vec2& speed)
{
	m_texcoords_spd = speed;
}

void OceanMesh::SetBlendSpeed(float spd)
{
	m_blend_speed = spd;
}

void OceanMesh::Update(float dt)
{
	if (m_uv_move_open) {
		UpdateUVMove(dt);
		Build();
	}
	if (m_wave_open) {
		UpdateWave(m_during);
	}
	if (m_blend_open) {
		UpdateBlend(dt);
	}
	m_during += dt;

	if (m_wave_speed * m_during > SM_PI * 2) {
		m_during = 0;
	}
}

void OceanMesh::Draw(const s2::RenderParams& params, bool draw_tris) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	if (m_blend_open && m_image1) {
		shader->SetColor(s2::Color(255, 255, 255, (int)(m_blend_base * 255 + 0.5f)).ToABGR(), 0);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			emesh::MeshRenderer::DrawMesh(m_grids[i], params, m_image0->GetTexID());
		}
		shader->SetColor(s2::Color(255, 255, 255, (int)((1 - m_blend_base) * 255 + 0.5f)).ToABGR(), 0);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			emesh::MeshRenderer::DrawMesh(m_grids[i], params, m_image1->GetTexID());
		}
	} else {
		shader->SetColor(0xffffffff, 0);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			emesh::MeshRenderer::DrawTexture(m_grids[i], params);
		}
	}

	if (draw_tris) {
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			emesh::MeshRenderer::DrawInfoXY(m_grids[i]);
		}
	}
}

void OceanMesh::OpenWave(bool open) 
{ 
	m_wave_open = open; 
}

void OceanMesh::OpenUVMove(bool open) 
{ 
	if (!open) {
		m_texcoords_base.Set(0, 0);
	}
	m_uv_move_open = open; 
}

void OceanMesh::OpenBlend(bool open) 
{ 
	if (!open) {
		m_blend_base = 0;
	}
	m_blend_open = open; 
}

void OceanMesh::SetImage1(const ee::ImageSymbol* image)
{
	sm::vec2 sz0 = m_image1->GetSize().Size(),
		     sz = image->GetSize().Size();
	if (sz0.x != sz.x || sz0.y != sz.y) {
		return;
	}

	m_image1 = image;
}

sm::rect OceanMesh::GetRegion() const
{
	sm::rect ret;
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		emesh::Mesh* shape = m_grids[i];
		ret.Combine(shape->GetRegion());
	}
	return ret;
}

float OceanMesh::GetTexcoordSpdAngle() const
{
	if (m_texcoords_spd.y == 0) {
		return SM_PI * 0.5f;
	} else {
		return atan(m_texcoords_spd.x / m_texcoords_spd.y);
	}
}

void OceanMesh::Rotate(float angle)
{
	std::vector<sm::vec2> vertices = m_shape->GetVertices();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] = ee::Math2D::RotateVector(vertices[i], angle);
	}
	m_shape->SetVertices(vertices);

	m_texcoords_spd.y = -sqrt(m_texcoords_spd.x * m_texcoords_spd.x 
		+ m_texcoords_spd.y * m_texcoords_spd.y);
	m_texcoords_spd.x = 0;
}

void OceanMesh::Clear()
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		m_grids[i]->Release();
	}
	m_grids.clear();
}

sm::rect OceanMesh::CalBoundRegion(const std::vector<sm::vec2>& bound) const
{
	sm::rect r;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		r.Combine(bound[i]);
	}
	debug_r = r;
	return r;
}

void OceanMesh::CalSegments(const sm::rect& r, std::vector<sm::vec2>& segs) const
{
	sm::vec2 sz = m_image0->GetSize().Size();
	float dw = sz.x / m_col,
		  dh = sz.y / m_row;
	for (float x = r.xmin; x < r.xmax; x += dw) {
		segs.push_back(sm::vec2(x, r.ymin - 1));
		segs.push_back(sm::vec2(x, r.ymax + 1));
	}
	for (float y = r.ymin; y < r.ymax; y += dh) {
		segs.push_back(sm::vec2(r.xmin - 1, y));
		segs.push_back(sm::vec2(r.xmax + 1, y));
	}

 	if (m_texcoords_base.x != 0) {
 		float offset = m_texcoords_base.x * sz.x;
		if (fabs(offset - (int)(offset / dw) * dw) > 1) {
			for (float x = r.xmin + offset; x < r.xmax; x += sz.x) {
				segs.push_back(sm::vec2(x, r.ymin - 1));
				segs.push_back(sm::vec2(x, r.ymax + 1));
			}
		}
 	}
 	if (m_texcoords_base.y != 0) {
 		float offset = m_texcoords_base.y * sz.y;
		if (fabs(offset - (int)(offset / dh) * dh) > 1) {
			for (float y = r.ymin + offset; y < r.ymax; y += sz.y) {
				segs.push_back(sm::vec2(r.xmin - 1, y));
				segs.push_back(sm::vec2(r.xmax + 1, y));
			}
		}
 	}
}

void OceanMesh::CalTrisTexcords(const sm::rect& r, 
								const std::vector<sm::vec2>& tris_vertices,
								std::vector<sm::vec2>& texcoords) const
{
	sm::vec2 sz = m_image0->GetSize().Size();

	sm::vec2 left_low;
	left_low.x = r.xmin - (1 - m_texcoords_base.x) * sz.x;
	left_low.y = r.ymin - (1 - m_texcoords_base.y) * sz.y;
	assert(tris_vertices.size() % 3 == 0);
	for (int i = 0, n = tris_vertices.size(); i < n; i += 3)
	{
		sm::vec2 center = tris_vertices[i] + tris_vertices[i+1] + tris_vertices[i+2];
		center /= 3;

		sm::vec2 base;
		int ix = (center.x - left_low.x) / sz.x,
			iy = (center.y - left_low.y) / sz.y;
		base.x = left_low.x + sz.x * ix;
		base.y = left_low.y + sz.y * iy;

		for (int j = 0; j < 3; ++j) {
			float tx = (tris_vertices[i+j].x - base.x) / sz.x,
				  ty = (tris_vertices[i+j].y - base.y) / sz.y;
			texcoords.push_back(sm::vec2(tx, ty));
		}
	}
}

void OceanMesh::BuildGrids(const sm::rect& region, 
						   const std::vector<sm::vec2>& vertices, 
						   const std::vector<sm::vec2>& texcoords,
						   const std::vector<sm::vec2>& bound)
{
	sm::vec2 sz = m_image0->GetSize().Size();
	sm::vec2 r_sz = region.Size();
	int cx = std::ceil(r_sz.x / sz.x),
		cy = std::ceil(r_sz.y / sz.y);
	std::vector<MeshShape*> grids;
	grids.resize(cx * cy, NULL);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		sm::vec2 center = vertices[i] + vertices[i+1] + vertices[i+2];
		center /= 3;

		int ix = (center.x - region.xmin) / sz.x,
			iy = (center.y - region.ymin) / sz.y;
		MeshShape** grid = &grids[iy * cx + ix];
		if (!*grid) {
			*grid = new MeshShape(m_image0);
		}
		(*grid)->InsertTriangle(&vertices[i], &texcoords[i], bound);
	}

	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]) {
			m_grids.push_back(grids[i]);
		}
	}
}

void OceanMesh::UpdateWave(float during)
{
	sm::vec2 sz = m_image0->GetSize().Size();
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		MeshShape* grid = m_grids[i];
		const std::vector<emesh::Triangle*>& tris = grid->GetTriangles();
		for (int j = 0, m = tris.size(); j < m; ++j) {
			emesh::Triangle* tri = tris[j];
			for (int k = 0; k < 3; ++k) {
				emesh::Node* n = tri->nodes[k];
				if (m_lock_bound && *((bool*)n->ud)) {
					continue;
				}

  				// todo
  				float x_times = (n->ori_xy.x - debug_r.xmin - m_texcoords_base.x * sz.x) / sz.x,
  					  y_times = (n->ori_xy.y - debug_r.ymin - m_texcoords_base.y * sz.y) / sz.y;
//   				if (fabs(x_times - (int)x_times) < 0.001f || 
//   					fabs(y_times - (int)y_times) < 0.001f) {
				if (fabs(y_times - (int)y_times) < 0.001f) {
//  					float dis = n->ori_xy.y - m_wave_speed * during;
//   					//n->xy.y = n->ori_xy.y;
//  					n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);

					float dis = n->ori_xy.y - m_texcoords_base.y * sz.y + m_wave_speed * during;
					n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
  				} else {
  					float dis = n->ori_xy.y - m_wave_speed * during;
  					n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
  				}

// 				float dis = n->ori_xy.y + m_texcoords_base.y * sz.y - m_wave_speed * during;
// 				n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);

// 				float dis = n->ori_xy.y - m_wave_speed * during;
// 				n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
			}
		}
	}
}

void OceanMesh::UpdateUVMove(float dt)
{
	m_texcoords_base += m_texcoords_spd * dt;

	if (m_texcoords_base.x >= 1) m_texcoords_base.x -= 1;
	if (m_texcoords_base.x < 0) m_texcoords_base.x += 1;
	if (m_texcoords_base.y >= 1) m_texcoords_base.y -= 1;
	if (m_texcoords_base.y < 0) m_texcoords_base.y += 1;
}

void OceanMesh::UpdateBlend(float dt)
{
	m_blend_base += m_blend_speed * dt;
	if (m_blend_base > 1) {
		m_blend_speed = -m_blend_speed;
		m_blend_base = 1;
	}
	if (m_blend_base < 0) {
		m_blend_speed = -m_blend_speed;
		m_blend_base = 0;
	}
}

}