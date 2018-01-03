#pragma once

#include <assert.h>

namespace eterrain3d
{

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