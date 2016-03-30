#include "OceanMesh.h"
#include "MeshShape.h"
#include "config.h"

#include <ee/Math2D.h>
#include <ee/Triangulation.h>
#include <ee/ShaderMgr.h>
#include <ee/SpriteShader.h>
#include <ee/ImageSymbol.h>

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

	std::vector<ee::Vector> bound;
	ee::Math2D::RemoveDuplicatePoints(m_shape->GetVertices(), bound);

	ee::Rect r = CalBoundRegion(bound);

	std::vector<ee::Vector> segs;
	CalSegments(r, segs);

	std::vector<ee::Vector> tris_vertices;
	ee::Triangulation::Lines(bound, segs, tris_vertices);
	if (tris_vertices.empty()) {
		return;
	}

	std::vector<ee::Vector> tris_texcoords;
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

void OceanMesh::SetTexcoordsSpeed(const ee::Vector& speed)
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

	if (m_wave_speed * m_during > ee::PI * 2) {
		m_during = 0;
	}
}

void OceanMesh::Draw(const ee::RenderParams& trans, bool draw_tris) const
{
	ee::ShaderMgr* mgr = ee::ShaderMgr::Instance();
	ee::SpriteShader* shader = static_cast<ee::SpriteShader*>(mgr->GetShader(ee::ShaderMgr::SPRITE));
	if (m_blend_open && m_image1) {
		ee::RenderColor color;
		color.multi = ee::Colorf(1, 1, 1, m_blend_base);
		shader->SetColor(color);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(trans, m_image0->GetTexID());
		}
		color.multi = ee::Colorf(1, 1, 1, 1 - m_blend_base);
		shader->SetColor(color);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(trans, m_image1->GetTexID());
		}
	} else {
		shader->SetColor(ee::RenderColor());
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(trans);
		}
	}

	if (draw_tris) {
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawInfoXY();
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
	if (m_image0->GetSize().Width() != image->GetSize().Width() ||
		m_image0->GetSize().Height() != image->GetSize().Height()) {
		return;
	}

	m_image1 = image;
}

ee::Rect OceanMesh::GetRegion() const
{
	ee::Rect ret;
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		emesh::Shape* shape = m_grids[i];
		ret.Combine(shape->GetRegion());
	}
	return ret;
}

float OceanMesh::GetTexcoordSpdAngle() const
{
	if (m_texcoords_spd.y == 0) {
		return ee::PI * 0.5f;
	} else {
		return atan(m_texcoords_spd.x / m_texcoords_spd.y);
	}
}

void OceanMesh::Rotate(float angle)
{
	std::vector<ee::Vector> vertices = m_shape->GetVertices();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] = ee::Math2D::RotateVector(vertices[i], angle);
	}
	m_shape->Load(vertices);

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

ee::Rect OceanMesh::CalBoundRegion(const std::vector<ee::Vector>& bound) const
{
	ee::Rect r;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		r.Combine(bound[i]);
	}
	debug_r = r;
	return r;
}

void OceanMesh::CalSegments(const ee::Rect& r, std::vector<ee::Vector>& segs) const
{
	int img_w = m_image0->GetSize().Width(),
		img_h = m_image0->GetSize().Height();
	float dw = img_w / m_col,
		  dh = img_h / m_row;
	for (float x = r.xmin; x < r.xmax; x += dw) {
		segs.push_back(ee::Vector(x, r.ymin - 1));
		segs.push_back(ee::Vector(x, r.ymax + 1));
	}
	for (float y = r.ymin; y < r.ymax; y += dh) {
		segs.push_back(ee::Vector(r.xmin - 1, y));
		segs.push_back(ee::Vector(r.xmax + 1, y));
	}

 	if (m_texcoords_base.x != 0) {
 		float offset = m_texcoords_base.x * img_w;
		if (fabs(offset - (int)(offset / dw) * dw) > 1) {
			for (float x = r.xmin + offset; x < r.xmax; x += img_w) {
				segs.push_back(ee::Vector(x, r.ymin - 1));
				segs.push_back(ee::Vector(x, r.ymax + 1));
			}
		}
 	}
 	if (m_texcoords_base.y != 0) {
 		float offset = m_texcoords_base.y * img_h;
		if (fabs(offset - (int)(offset / dh) * dh) > 1) {
			for (float y = r.ymin + offset; y < r.ymax; y += img_h) {
				segs.push_back(ee::Vector(r.xmin - 1, y));
				segs.push_back(ee::Vector(r.xmax + 1, y));
			}
		}
 	}
}

void OceanMesh::CalTrisTexcords(const ee::Rect& r, 
								const std::vector<ee::Vector>& tris_vertices,
								std::vector<ee::Vector>& texcoords) const
{
	float img_w = m_image0->GetSize().Width(),
		  img_h = m_image0->GetSize().Height();

	ee::Vector left_low;
	left_low.x = r.xmin - (1 - m_texcoords_base.x) * img_w;
	left_low.y = r.ymin - (1 - m_texcoords_base.y) * img_h;
	assert(tris_vertices.size() % 3 == 0);
	for (int i = 0, n = tris_vertices.size(); i < n; i += 3)
	{
		ee::Vector center = tris_vertices[i] + tris_vertices[i+1] + tris_vertices[i+2];
		center /= 3;

		ee::Vector base;
		int ix = (center.x - left_low.x) / img_w,
			iy = (center.y - left_low.y) / img_h;
		base.x = left_low.x + img_w * ix;
		base.y = left_low.y + img_h * iy;

		for (int j = 0; j < 3; ++j) {
			float tx = (tris_vertices[i+j].x - base.x) / img_w,
				  ty = (tris_vertices[i+j].y - base.y) / img_h;
			texcoords.push_back(ee::Vector(tx, ty));
		}
	}
}

void OceanMesh::BuildGrids(const ee::Rect& region, 
						   const std::vector<ee::Vector>& vertices, 
						   const std::vector<ee::Vector>& texcoords,
						   const std::vector<ee::Vector>& bound)
{
	float img_w = m_image0->GetSize().Width(),
		  img_h = m_image0->GetSize().Height();
	int cx = std::ceil(region.Width() / img_w),
		cy = std::ceil(region.Height() / img_h);
	std::vector<MeshShape*> grids;
	grids.resize(cx * cy, NULL);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		ee::Vector center = vertices[i] + vertices[i+1] + vertices[i+2];
		center /= 3;

		int ix = (center.x - region.xmin) / img_w,
			iy = (center.y - region.ymin) / img_h;
		MeshShape** grid = &grids[iy * cx + ix];
		if (!*grid) {
			*grid = new MeshShape(*m_image0->GetImage());
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
	int img_w = m_image0->GetSize().Width(),
		img_h = m_image0->GetSize().Height();
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
  				float x_times = (n->ori_xy.x - debug_r.xmin - m_texcoords_base.x * img_w) / img_w,
  					  y_times = (n->ori_xy.y - debug_r.ymin - m_texcoords_base.y * img_h) / img_h;
//   				if (fabs(x_times - (int)x_times) < 0.001f || 
//   					fabs(y_times - (int)y_times) < 0.001f) {
				if (fabs(y_times - (int)y_times) < 0.001f) {
//  					float dis = n->ori_xy.y - m_wave_speed * during;
//   					//n->xy.y = n->ori_xy.y;
//  					n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);

					float dis = n->ori_xy.y - m_texcoords_base.y * img_h + m_wave_speed * during;
					n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
  				} else {
  					float dis = n->ori_xy.y - m_wave_speed * during;
  					n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
  				}

// 				float dis = n->ori_xy.y + m_texcoords_base.y * img_h - m_wave_speed * during;
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