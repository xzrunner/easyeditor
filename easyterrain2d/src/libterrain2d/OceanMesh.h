#ifndef _EASYTERRAIN2D_OCEAN_MESH_H_
#define _EASYTERRAIN2D_OCEAN_MESH_H_

#include <easyshape.h>

namespace eterrain2d
{

class MeshShape;

class OceanMesh : public d2d::Object
{
public:
	OceanMesh(const libshape::PolygonShape* shape, const d2d::ImageSymbol* image);
	virtual ~OceanMesh();

	void Build();

	const libshape::PolygonShape* GetBounding() const { return m_shape; }
	const d2d::ImageSymbol* GetImage0() const { return m_image0; }
	const d2d::ImageSymbol* GetImage1() const { return m_image1; }
	bool IsWaveOpen() const { return m_wave_open; }
	int GetWaveGridRow() const { return m_row; }
	int GetWaveGridCol() const { return m_col; }
	float GetWaveSpeed() const { return m_wave_speed; }
	float GetWaveHeight() const { return m_wave_height; }
	bool IsUVMoveOpen() const { return m_uv_move_open; }
	const d2d::Vector& GetUVMoveSpeed() const { return m_texcoords_spd; }
	bool IsTexBlendOpen() const { return m_blend_open; }
	float GetTexBlendSpeed() const { return m_blend_speed; }

	void SetSmallGridSize(int row, int col);
	void SetWaveInfo(float speed, float height);
	void SetTexcoordsSpeed(const d2d::Vector& speed);
	void SetBlendSpeed(float spd);

	void Update(float dt);
	void Draw(const d2d::Matrix& mt, const d2d::Colorf& mul, 
		const d2d::Colorf& add, bool draw_tris) const;

	const libshape::PolygonShape* GetShape() const {
		return m_shape;
	}

	void OpenWave(bool open);
	void OpenUVMove(bool open);
	void OpenBlend(bool open);

	bool IsBlendOpen() const { return m_blend_open; }
	void SetImage1(const d2d::ImageSymbol* image);

	d2d::Rect GetRegion() const;

	void SetBoundLock(bool lock) { m_lock_bound = lock; }
	bool IsBoundLockOpen() const { return m_lock_bound; }

	const std::vector<MeshShape*>& GetMeshes() const { return m_grids; }

	float GetTexcoordSpdAngle() const;
	void Rotate(float angle);

private:
	void Clear();

	d2d::Rect CalBoundRegion(const std::vector<d2d::Vector>& bound); 
	void CalSegments(const d2d::Rect& region, std::vector<d2d::Vector>& segs);
	void CalTrisTexcords(const d2d::Rect& region, const std::vector<d2d::Vector>& vertices,
		std::vector<d2d::Vector>& texcoords);
	void BuildGrids(const d2d::Rect& region, const std::vector<d2d::Vector>& vertices, 
		const std::vector<d2d::Vector>& texcoords, const std::vector<d2d::Vector>& bound);

	void UpdateWave(float during);
	void UpdateUVMove(float dt);
	void UpdateBlend(float dt);

private:
	libshape::PolygonShape* m_shape;
	const d2d::ImageSymbol *m_image0, *m_image1;

	std::vector<MeshShape*> m_grids;

	bool m_wave_open;
	int m_row, m_col;
	float m_wave_speed, m_wave_height;

	bool m_uv_move_open;
	d2d::Vector m_texcoords_base, m_texcoords_spd;

	bool m_blend_open;
	float m_blend_base, m_blend_speed;

	bool m_lock_bound;

	// debug
	d2d::Rect debug_r;

	float m_during;

}; // OceanMesh

}

#endif // _EASYTERRAIN2D_OCEAN_MESH_H_