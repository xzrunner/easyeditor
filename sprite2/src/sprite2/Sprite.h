#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "AABB.h"
#include "Color.h"
#include "FilterMode.h"
#include "BlendMode.h"
#include "CameraMode.h"

#include <SM_Vector.h>

namespace s2
{

class Sprite
{
public:
	Sprite();
	
	virtual bool Update(float dt) const = 0;

protected:
	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/

	sm::vec2	m_position;
	float		m_angle;
	sm::vec2	m_scale;
	sm::vec2	m_shear;

	AABB		m_aabb;
	
	// todo mat

	/************************************************************************/
	/* draw                                                                 */
	/************************************************************************/

	Color		m_col_mul, m_col_add;
	Color		m_col_rmap, m_col_gmap, m_col_bmap;

	FilterMode	m_filter;
	BlendMode	m_blend;

	CameraMode	m_camera;
	float		m_camera_base_y;	

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_