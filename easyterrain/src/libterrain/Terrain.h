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
	enum ETILE_TYPES
	{
		LOWEST_TILE= 0,		//sand, dirt, etc.
		LOW_TILE,			//grass
		HIGH_TILE,			//mountainside
		HIGHEST_TILE		//tip of mountain
	};

public:
	Terrain();
	~Terrain();

	virtual void Draw() const = 0;

	bool LoadHeightMap(const char* filename, int size);
	bool SaveHeightMap(const char* filename);

	bool MakeTerrainFault( int iSize, int iIterations, int iMinDelta, int iMaxDelta, float fFilter );
	bool MakeTerrainPlasma( int iSize, float fRoughness );

	//texture map generation
	void GenerateTextureMap( unsigned int uiSize );

	bool LoadTexture(char* filename);
	bool LoadTile( ETILE_TYPES tileType, char* szFilename );

	unsigned char GetTrueHeightAtPoint(int x, int y) const;
	float GetScaledHeightAtPoint(int x, int y) const;

	void SetHeightScale(float scale);

	void SetHeightAtPoint(unsigned char height, int x, int y);

private:
	//fractal terrain generation
	void NormalizeTerrain( float* fpHeightData );
	void FilterHeightBand( float* fpBand, int iStride, int iCount, float fFilter );
	void FilterHeightField( float* fpHeightData, float fFilter );

	//texture map generation functions
	float RegionPercent( int tileType, unsigned char ucHeight );
	void GetTexCoords( CIMAGE texture, unsigned int* x, unsigned int* y );
	unsigned char InterpolateHeight( int x, int z, float fHeightToTexRatio );

private:
	struct HeightData
	{
		unsigned char* pixels;
		int size;

		HeightData() : pixels(NULL), size(0) {}
	};

	struct TextureRegions
	{
		int m_iLowHeight;			//lowest possible height (0%)
		int m_iOptimalHeight;		//optimal height (100%)
		int m_iHighHeight;			//highest possible height (0%)
	};

	static const int TRN_NUM_TILES = 5;
	struct TextureTiles
	{
		TextureRegions m_regions[TRN_NUM_TILES];	//texture regions
		CIMAGE textureTiles[TRN_NUM_TILES];			//texture tiles
		int iNumTiles;
	};

protected:
	HeightData m_data;

	float m_hight_scale;

	CIMAGE m_texture;
	TextureTiles m_tiles;

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