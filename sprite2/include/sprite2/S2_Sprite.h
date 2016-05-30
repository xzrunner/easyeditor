#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "AABB.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"
#include "RefCountObj.h"

#include <SM_Vector.h>

namespace s2
{

class Symbol;

class Sprite : public RefCountObj
{
public:
	Sprite(void* ud);
	Sprite(const Sprite& spr, void* ud);
	virtual ~Sprite();
	
	virtual bool Update(float dt) = 0;

	void* GetUD() { return m_ud; }

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/

	const sm::vec2&		GetPosition() const	{ return m_position; }
	const float&		GetAngle() const	{ return m_angle; }
	const sm::vec2&		GetScale() const	{ return m_scale; }
	const sm::vec2&		GetShear() const	{ return m_shear; }

	const RenderColor&	Color() const		{ return m_color; }
	RenderColor&		Color()				{ return m_color; }
	const RenderShader& Shader() const		{ return m_shader; }
	RenderShader&		Shader()			{ return m_shader; }
	const RenderCamera& Camera() const		{ return m_camera; }
	RenderCamera&		Camera()			{ return m_camera; }

	void SetPosition(const sm::vec2& pos);
	void SetAngle(float angle);
	void SetScale(const sm::vec2& scale);
	void SetShear(const sm::vec2& shear);

protected:
	Sprite();
	Sprite(const Sprite& spr);
	const Sprite& operator = (const Sprite& spr) { return *this; }

//	void MultiplyRenderParams(const RenderParams& src, RenderParams& dst) const;

private:
	void UpdateBounding();

protected:
	Symbol*			m_sym;

private:
	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	sm::vec2		m_position;
	float			m_angle;
	sm::vec2		m_scale;
	sm::vec2		m_shear;

	sm::vec2		m_offset;

	Bounding*		m_bounding;
	bool			m_bounding_dirty;

	// todo mat

	/************************************************************************/
	/* draw                                                                 */
	/************************************************************************/
	RenderColor		m_color;
	RenderShader	m_shader;
	RenderCamera	m_camera;

	void*			m_ud;	// for extend

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_