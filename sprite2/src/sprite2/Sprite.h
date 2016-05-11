#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "AABB.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <SM_Vector.h>

namespace s2
{

class Sprite
{
public:
	Sprite();
	
	virtual bool Update(float dt) = 0;

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
	RenderColor	 m_color;
	RenderShader m_shader;
	RenderCamera m_camera;

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_