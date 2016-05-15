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
	Sprite(void* ud);
	
	virtual bool Update(float dt) = 0;
	
	void* GetUD() { return m_ud; }

#ifdef S2_EXTEND
	const sm::vec2& Position() const	{ return m_position; }
	sm::vec2& Position()				{ return m_position; }
	const float& Angle() const			{ return m_angle; }
	float& Angle()						{ return m_angle; }
	const sm::vec2& Scale() const		{ return m_scale; }
	sm::vec2& Scale()					{ return m_scale; }
	const sm::vec2& Shear() const		{ return m_shear; }
	sm::vec2& Shear()					{ return m_shear; }

	const RenderColor& Color() const	{ return m_color; }
	RenderColor& Color()				{ return m_color; }
	const RenderShader& Shader() const	{ return m_shader; }
	RenderShader& Shader()				{ return m_shader; }
	const RenderCamera& Camera() const	{ return m_camera; }
	RenderCamera& Camera()				{ return m_camera; }
#endif // S2_EXTEND

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

	void* m_ud;	// for extend

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_