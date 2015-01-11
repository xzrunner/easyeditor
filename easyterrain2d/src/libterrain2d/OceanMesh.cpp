#include "OceanMesh.h"
#include "MeshShape.h"
#include "config.h"

namespace eterrain2d
{

OceanMesh::OceanMesh(const libshape::PolygonShape* shape, 
					 const d2d::ImageSymbol* image)
	 : m_shape(shape)
	 , m_image(image)
{
	m_row = MESH_ROW;
	m_col = MESH_COL;

	m_wave_speed = WAVE_SPEED * 0.1f;
	m_wave_height = WAVE_HEIGHT;

	Build();
}

OceanMesh::~OceanMesh()
{
	Clear();
}

void OceanMesh::Build()
{
	Clear();

	std::vector<d2d::Vector> bound;
	d2d::Math::removeDuplicatePoints(m_shape->GetVertices(), bound);

	d2d::Rect r = CalBoundRegion(bound);

	std::vector<d2d::Vector> segs;
	CalSegments(r, segs);

	std::vector<d2d::Vector> tris_vertices;
	d2d::Triangulation::lines(bound, segs, tris_vertices);
	if (tris_vertices.empty()) {
		return;
	}

	std::vector<d2d::Vector> tris_texcoords;
	CalTrisTexcords(r, tris_vertices, tris_texcoords);

	BuildGrids(r, tris_vertices, tris_texcoords);
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

void OceanMesh::Update(float dt)
{
	static float during = 0;

	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		MeshShape* grid = m_grids[i];
		const std::vector<emesh::Triangle*>& tris = grid->GetTriangles();
		for (int j = 0, m = tris.size(); j < m; ++j) {
			emesh::Triangle* tri = tris[j];
			for (int k = 0; k < 3; ++k) {
				emesh::Node* n = tri->nodes[k];
				float dis = n->ori_xy.y - m_wave_speed * during;
				n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
			}
		}
	}

	during += dt;
}

void OceanMesh::Draw(bool draw_tris) const
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		MeshShape* grid = m_grids[i];
		grid->DrawTexture(d2d::Matrix());
		if (draw_tris) {
			grid->DrawInfoXY();
		}
	}
}

void OceanMesh::Clear()
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		m_grids[i]->Release();
	}
	m_grids.clear();
}

d2d::Rect OceanMesh::CalBoundRegion(const std::vector<d2d::Vector>& bound)
{
	d2d::Rect r;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		r.combine(bound[i]);
	}
	return r;
}

void OceanMesh::CalSegments(const d2d::Rect& r, std::vector<d2d::Vector>& segs)
{
	int dw = m_image->getSize().xLength() / m_col,
		dh = m_image->getSize().yLength() / m_row;
	for (float x = r.xMin; x < r.xMax; x += dw) {
		segs.push_back(d2d::Vector(x, r.yMin - 1));
		segs.push_back(d2d::Vector(x, r.yMax + 1));
	}
	for (float y = r.yMin; y < r.yMax; y += dh) {
		segs.push_back(d2d::Vector(r.xMin - 1, y));
		segs.push_back(d2d::Vector(r.xMax + 1, y));
	}
}

void OceanMesh::CalTrisTexcords(const d2d::Rect& r, 
								const std::vector<d2d::Vector>& tris_vertices,
								std::vector<d2d::Vector>& texcoords)
{
	float img_w = m_image->getSize().xLength(),
		  img_h = m_image->getSize().yLength();

	assert(tris_vertices.size() % 3 == 0);
	for (int i = 0, n = tris_vertices.size(); i < n; i += 3)
	{
		d2d::Vector center = tris_vertices[i] + tris_vertices[i+1] + tris_vertices[i+2];
		center /= 3;

		d2d::Vector base;
		int ix = (center.x - r.xMin) / img_w,
			iy = (center.y - r.yMin) / img_h;
		base.x = r.xMin + img_w * ix;
		base.y = r.yMin + img_h * iy;

		for (int j = 0; j < 3; ++j) {
			float tx = (tris_vertices[i+j].x - base.x) / img_w,
				  ty = (tris_vertices[i+j].y - base.y) / img_h;
			texcoords.push_back(d2d::Vector(tx, ty));
		}
	}
}

void OceanMesh::BuildGrids(const d2d::Rect& region, 
						   const std::vector<d2d::Vector>& vertices, 
						   const std::vector<d2d::Vector>& texcoords)
{
	float img_w = m_image->getSize().xLength(),
		  img_h = m_image->getSize().yLength();
	int cx = std::ceil(region.xLength() / img_w),
		cy = std::ceil(region.yLength() / img_h);
	std::vector<MeshShape*> grids;
	grids.resize(cx * cy, NULL);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		d2d::Vector center = vertices[i] + vertices[i+1] + vertices[i+2];
		center /= 3;

		int ix = (center.x - region.xMin) / img_w,
			iy = (center.y - region.yMin) / img_h;
		MeshShape** grid = &grids[iy * cx + ix];
		if (!*grid) {
			*grid = new MeshShape(*m_image->getImage());
		}
		(*grid)->InsertTriangle(&vertices[i], &texcoords[i]);
	}

	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]) {
			m_grids.push_back(grids[i]);
		}
	}
}

}