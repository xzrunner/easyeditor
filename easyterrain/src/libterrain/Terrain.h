//
// From "Focus On 3D Teeeain Programming" source code, 
// by Trent Polack (trent@voxelsoft.com)
//

#ifndef _EASYTERRAIN_TERRAIN_H_
#define _EASYTERRAIN_TERRAIN_H_

#include <easy3d.h>

#include "image.h"

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

	bool MakeTerrainFault( int iSize, int iIterations, int iMinDelta, int iMaxDelta, float fFilter );
	bool MakeTerrainPlasma( int iSize, float fRoughness );

	bool LoadTexture(char* filename);

	unsigned char GetTrueHeightAtPoint(int x, int y) const;
	float GetScaledHeightAtPoint(int x, int y) const;

	void SetHeightScale(float scale);

	void SetHeightAtPoint(unsigned char height, int x, int y);

private:
	//fractal terrain generation
	void NormalizeTerrain( float* fpHeightData );
	void FilterHeightBand( float* fpBand, int iStride, int iCount, float fFilter );
	void FilterHeightField( float* fpHeightData, float fFilter );

private:
	struct HeightData
	{
		unsigned char* pixels;
		int size;

		HeightData() : pixels(NULL), size(0) {}
	};

protected:
	HeightData m_data;

	CIMAGE m_texture;

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

inline void 
Terrain::SetHeightAtPoint(unsigned char height, int x, int y)
{	
	assert(x >= 0 && x < m_data.size 
		&& y >= 0 && y < m_data.size);
	m_data.pixels[(y * m_data.size) + x] = height;
}


}

#endif // _EASYTERRAIN_TERRAIN_H_