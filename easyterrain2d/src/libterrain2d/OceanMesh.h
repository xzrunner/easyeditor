#ifndef _EASYTERRAIN2D_OCEAN_MESH_H_
#define _EASYTERRAIN2D_OCEAN_MESH_H_

#include <easyshape.h>

namespace eterrain2d
{

class MeshShape;

class OceanMesh : public ee::Object
{
public:
	OceanMesh(const eshape::PolygonShape* shape, const ee::ImageSymbol* image);
	virtual ~OceanMesh();

	void Build();

	const eshape::PolygonShape* GetBounding() const { return m_shape; }
	const ee::ImageSymbol* GetImage0() const { return m_image0; }
	const ee::ImageSymbol* GetImage1() const { return m_image1; }
	bool IsWaveOpen() const { return m_wave_open; }
	int GetWaveGridRow() const { return m_row; }
	int GetWaveGridCol() const { return m_col; }
	float GetWaveSpeed() const { return m_wave_speed; }
	float GetWaveHeight() const { return m_wave_height; }
	bool IsUVMoveOpen() const { return m_uv_move_open; }
	const sm::vec2& GetUVMoveSpeed() const { return m_texcoords_spd; }
	bool IsTexBlendOpen() const { return m_blend_open; }
	float GetTexBlendSpeed() const { return m_blend_speed; }

	void SetSmallGridSize(int row, int col);
	void SetWaveInfo(float speed, float height);
	void SetTexcoordsSpeed(const sm::vec2& speed);
	void SetBlendSpeed(float spd);

	void Update(float dt);
	void Draw(const ee::RenderParams& trans, bool draw_tris) const;

	const eshape::PolygonShape* GetShape() const {
		return m_shape;
	}

	void OpenWave(bool open);
	void OpenUVMove(bool open);
	void OpenBlend(bool open);

	bool IsBlendOpen() const { return m_blend_open; }
	void SetImage1(const ee::ImageSymbol* image);

	ee::Rect GetRegion() const;

	void SetBoundLock(bool lock) { m_lock_bound = lock; }
	bool IsBoundLockOpen() const { return m_lock_bound; }

	const std::vector<MeshShape*>& GetMeshes() const { return m_grids; }

	float GetTexcoordSpdAngle() const;
	void Rotate(float angle);

private:
	void Clear();

	ee::Rect CalBoundRegion(const std::vector<sm::vec2>& bound) const; 
	void CalSegments(const ee::Rect& region, std::vector<sm::vec2>& segs) const;
	void CalTrisTexcords(const ee::Rect& region, const std::vector<sm::vec2>& vertices,
		std::vector<sm::vec2>& texcoords) const;
	void BuildGrids(const ee::Rect& region, const std::vector<sm::vec2>& vertices, 
		const std::vector<sm::vec2>& texcoords, const std::vector<sm::vec2>& bound);

	void UpdateWave(float during);
	void UpdateUVMove(float dt);
	void UpdateBlend(float dt);

private:
	eshape::PolygonShape* m_shape;
	const ee::ImageSymbol *m_image0, *m_image1;

	std::vector<MeshShape*> m_grids;

	bool m_wave_open;
	int m_row, m_col;
	float m_wave_speed, m_wave_height;

	bool m_uv_move_open;
	sm::vec2 m_texcoords_base, m_texcoords_spd;

	bool m_blend_open;
	float m_blend_base, m_blend_speed;

	bool m_lock_bound;

	// debug
	mutable ee::Rect debug_r;

	float m_during;

}; // OceanMesh

}

#endif // _EASYTERRAIN2D_OCEAN_MESH_H_