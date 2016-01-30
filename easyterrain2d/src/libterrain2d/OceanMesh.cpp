#include "OceanMesh.h"
#include "MeshShape.h"
#include "config.h"

namespace eterrain2d
{

OceanMesh::OceanMesh(const libshape::PolygonShape* shape, 
					 const d2d::ImageSymbol* image)
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

	std::vector<d2d::Vector> bound;
	d2d::Math2D::RemoveDuplicatePoints(m_shape->GetVertices(), bound);

	d2d::Rect r = CalBoundRegion(bound);

	std::vector<d2d::Vector> segs;
	CalSegments(r, segs);

	std::vector<d2d::Vector> tris_vertices;
	d2d::Triangulation::Lines(bound, segs, tris_vertices);
	if (tris_vertices.empty()) {
		return;
	}

	std::vector<d2d::Vector> tris_texcoords;
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

void OceanMesh::SetTexcoordsSpeed(const d2d::Vector& speed)
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

	if (m_wave_speed * m_during > d2d::PI * 2) {
		m_during = 0;
	}
}

void OceanMesh::Draw(const d2d::Matrix& mt, const d2d::Colorf& mul, 
					 const d2d::Colorf& add, bool draw_tris) const
{
	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	if (m_blend_open && m_image1) {
		d2d::ColorTrans color;
		color.multi = d2d::Colorf(1, 1, 1, m_blend_base);
		shader->SetSpriteColor(color);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(mt, mul, add, m_image0->GetTexID());
		}
		color.multi = d2d::Colorf(1, 1, 1, 1 - m_blend_base);
		shader->SetSpriteColor(color);
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(mt, mul, add, m_image1->GetTexID());
		}
	} else {
		shader->SetSpriteColor(d2d::ColorTrans());
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(mt, mul, add);
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

void OceanMesh::SetImage1(const d2d::ImageSymbol* image)
{
	if (m_image0->GetSize().Width() != image->GetSize().Width() ||
		m_image0->GetSize().Height() != image->GetSize().Height()) {
		return;
	}

	m_image1 = image;
}

d2d::Rect OceanMesh::GetRegion() const
{
	d2d::Rect ret;
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		emesh::Shape* shape = m_grids[i];
		ret.Combine(shape->GetRegion());
	}
	return ret;
}

float OceanMesh::GetTexcoordSpdAngle() const
{
	if (m_texcoords_spd.y == 0) {
		return d2d::PI * 0.5f;
	} else {
		return atan(m_texcoords_spd.x / m_texcoords_spd.y);
	}
}

void OceanMesh::Rotate(float angle)
{
	std::vector<d2d::Vector> vertices = m_shape->GetVertices();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] = d2d::Math2D::RotateVector(vertices[i], angle);
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

d2d::Rect OceanMesh::CalBoundRegion(const std::vector<d2d::Vector>& bound) const
{
	d2d::Rect r;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		r.Combine(bound[i]);
	}
	debug_r = r;
	return r;
}

void OceanMesh::CalSegments(const d2d::Rect& r, std::vector<d2d::Vector>& segs) const
{
	int img_w = m_image0->GetSize().Width(),
		img_h = m_image0->GetSize().Height();
	float dw = img_w / m_col,
		  dh = img_h / m_row;
	for (float x = r.xmin; x < r.xmax; x += dw) {
		segs.push_back(d2d::Vector(x, r.ymin - 1));
		segs.push_back(d2d::Vector(x, r.ymax + 1));
	}
	for (float y = r.ymin; y < r.ymax; y += dh) {
		segs.push_back(d2d::Vector(r.xmin - 1, y));
		segs.push_back(d2d::Vector(r.xmax + 1, y));
	}

 	if (m_texcoords_base.x != 0) {
 		float offset = m_texcoords_base.x * img_w;
		if (fabs(offset - (int)(offset / dw) * dw) > 1) {
			for (float x = r.xmin + offset; x < r.xmax; x += img_w) {
				segs.push_back(d2d::Vector(x, r.ymin - 1));
				segs.push_back(d2d::Vector(x, r.ymax + 1));
			}
		}
 	}
 	if (m_texcoords_base.y != 0) {
 		float offset = m_texcoords_base.y * img_h;
		if (fabs(offset - (int)(offset / dh) * dh) > 1) {
			for (float y = r.ymin + offset; y < r.ymax; y += img_h) {
				segs.push_back(d2d::Vector(r.xmin - 1, y));
				segs.push_back(d2d::Vector(r.xmax + 1, y));
			}
		}
 	}
}

void OceanMesh::CalTrisTexcords(const d2d::Rect& r, 
								const std::vector<d2d::Vector>& tris_vertices,
								std::vector<d2d::Vector>& texcoords) const
{
	float img_w = m_image0->GetSize().Width(),
		  img_h = m_image0->GetSize().Height();

	d2d::Vector left_low;
	left_low.x = r.xmin - (1 - m_texcoords_base.x) * img_w;
	left_low.y = r.ymin - (1 - m_texcoords_base.y) * img_h;
	assert(tris_vertices.size() % 3 == 0);
	for (int i = 0, n = tris_vertices.size(); i < n; i += 3)
	{
		d2d::Vector center = tris_vertices[i] + tris_vertices[i+1] + tris_vertices[i+2];
		center /= 3;

		d2d::Vector base;
		int ix = (center.x - left_low.x) / img_w,
			iy = (center.y - left_low.y) / img_h;
		base.x = left_low.x + img_w * ix;
		base.y = left_low.y + img_h * iy;

		for (int j = 0; j < 3; ++j) {
			float tx = (tris_vertices[i+j].x - base.x) / img_w,
				  ty = (tris_vertices[i+j].y - base.y) / img_h;
			texcoords.push_back(d2d::Vector(tx, ty));
		}
	}
}

void OceanMesh::BuildGrids(const d2d::Rect& region, 
						   const std::vector<d2d::Vector>& vertices, 
						   const std::vector<d2d::Vector>& texcoords,
						   const std::vector<d2d::Vector>& bound)
{
	float img_w = m_image0->GetSize().Width(),
		  img_h = m_image0->GetSize().Height();
	int cx = std::ceil(region.Width() / img_w),
		cy = std::ceil(region.Height() / img_h);
	std::vector<MeshShape*> grids;
	grids.resize(cx * cy, NULL);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		d2d::Vector center = vertices[i] + vertices[i+1] + vertices[i+2];
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