#ifndef _EASYTERRAIN2D_OCEAN_MESH_H_
#define _EASYTERRAIN2D_OCEAN_MESH_H_

#include <easyshape.h>

namespace eterrain2d
{

class MeshShape;

class OceanMesh
{
public:
	OceanMesh(const libshape::PolygonShape* shape, const d2d::ImageSymbol* image);
	virtual ~OceanMesh();

	void Build();

	void SetSmallGridSize(int row, int col);
	void SetWaveInfo(float speed, float height);
	void SetTexcoordsSpeed(const d2d::Vector& speed);

	void Update(float dt);
	void Draw(bool draw_tris) const;

	const libshape::PolygonShape* GetShape() const {
		return m_shape;
	}

	void OpenWave(bool open) { m_wave_open = open; }
	void OpenUVMove(bool open) { m_uv_move_open = open; }

private:
	void Clear();

	d2d::Rect CalBoundRegion(const std::vector<d2d::Vector>& bound); 
	void CalSegments(const d2d::Rect& region, std::vector<d2d::Vector>& segs);
	void CalTrisTexcords(const d2d::Rect& region, const std::vector<d2d::Vector>& vertices,
		std::vector<d2d::Vector>& texcoords);
	void BuildGrids(const d2d::Rect& region, const std::vector<d2d::Vector>& vertices, 
		const std::vector<d2d::Vector>& texcoords);

	void UpdateWave(float during);
	void UpdateUVMove(float dt);

private:
	const libshape::PolygonShape* m_shape;
	const d2d::ImageSymbol* m_image;

	std::vector<MeshShape*> m_grids;
 
	int m_row, m_col;

	bool m_wave_open;
	float m_wave_speed, m_wave_height;

	bool m_uv_move_open;
	d2d::Vector m_texcoords_spd;
	d2d::Vector m_texcoords_base;

}; // OceanMesh

}

#endif // _EASYTERRAIN2D_OCEAN_MESH_H_