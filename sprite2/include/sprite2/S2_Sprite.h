#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "pre_defined.h"
#include "AABB.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <SM_Vector.h>
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

namespace s2
{

class RenderParams;
class Symbol;

class Sprite : public cu::RefCountObj, public cu::Cloneable
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	virtual ~Sprite();
	
	virtual bool Update(const RenderParams& params, float dt) { return false; }

	bool IsVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }

	const Bounding* GetBounding() const { return m_bounding; }

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
	const Sprite& operator = (const Sprite& spr);

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

	/************************************************************************/
	/* message                                                              */
	/************************************************************************/
	bool			m_visible;

// 	/************************************************************************/
// 	/* extend                                                               */
// 	/************************************************************************/
// 	int				m_flags;
//	void*			m_ud;

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_