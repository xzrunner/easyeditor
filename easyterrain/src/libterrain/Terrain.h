#ifndef _EASYTERRAIN_TERRAIN_H_
#define _EASYTERRAIN_TERRAIN_H_

#include <easy3d.h>

namespace libterrain
{

class Terrain
{
public:
	Terrain();
	~Terrain();

	virtual void Draw() const = 0;

	bool LoadHeightMap(const char* filename, int size);
	bool SaveHeightMap(const char* filename);

	unsigned char GetTrueHeightAtPoint(int x, int y) const;
	float GetScaledHeightAtPoint(int x, int y) const;

	void SetHeightScale(float scale);

private:
	struct HeightData
	{
		unsigned char* pixels;
		int size;

		HeightData() : pixels(NULL), size(0) {}
	};

protected:
	HeightData m_data;

	float m_hight_scale;

}; // Terrain

inline unsigned char 
Terrain::GetTrueHeightAtPoint(int x, int y) const
{
	assert(x >= 0 && x < m_data.size 
		&& y >= 0 && y < m_data.size);
	return m_data.pixels[(y * m_data.size) + x];
}

inline float 
Terrain::GetScaledHeightAtPoint(int x, int y) const
{
	assert(x >= 0 && x < m_data.size 
		&& y >= 0 && y < m_data.size);
	return (float)m_data.pixels[(y * m_data.size) + x] * m_hight_scale;
}

inline void 
Terrain::SetHeightScale(float scale)
{
	m_hight_scale = scale;
}

}

#endif // _EASYTERRAIN_TERRAIN_H_