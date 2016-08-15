#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "pre_defined.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

namespace s2
{

class RenderParams;
class Symbol;
class BoundingBox;

class Sprite : public cu::RefCountObj, public cu::Cloneable
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite(Symbol* sym);
	virtual ~Sprite();

	/**
	 *  @interface
	 *    cu::Cloneable
	 */
	virtual Sprite* Clone() const { return NULL; }

	virtual void Translate(const sm::vec2& offset);
	virtual void Rotate(float delta);
	
	virtual bool Update(const RenderParams& params, float dt) { return false; }

	virtual void SetSymbol(Symbol* sym);

	Symbol* GetSymbol() { return m_sym; }
	const Symbol* GetSymbol() const { return m_sym; }

	bool IsVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }

	const BoundingBox* GetBounding() const;
	void UpdateBounding() const;

	sm::vec2 GetCenter() const;
	const sm::vec2&		GetPosition() const	{ return m_position; }
	const float&		GetAngle() const	{ return m_angle; }
	const sm::vec2&		GetScale() const	{ return m_scale; }
	const sm::vec2&		GetShear() const	{ return m_shear; }
	const sm::vec2&		GetOffset() const	{ return m_offset; }

	void SetPosition(const sm::vec2& pos);
	void SetAngle(float angle);
	void SetScale(const sm::vec2& scale);
	void SetShear(const sm::vec2& shear);
	void SetOffset(const sm::vec2& offset);

	const RenderColor&	Color() const		{ return m_color; }
	RenderColor&		Color()				{ return m_color; }
	const RenderShader& Shader() const		{ return m_shader; }
	RenderShader&		Shader()			{ return m_shader; }
	const RenderCamera& Camera() const		{ return m_camera; }
	RenderCamera&		Camera()			{ return m_camera; }

	sm::mat4 GetTransMatrix() const;
	sm::mat4 GetTransInvMatrix() const;

protected:
	const Sprite& operator = (const Sprite& spr);

protected:
	Symbol*					m_sym;

	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	sm::vec2				m_position;
	float					m_angle;
	sm::vec2				m_scale;
	sm::vec2				m_shear;

	mutable sm::vec2		m_offset;

	mutable BoundingBox*	m_bounding;
	mutable bool			m_bounding_dirty;

	// todo mat

	/************************************************************************/
	/* draw                                                                 */
	/************************************************************************/
	RenderColor				m_color;
	RenderShader			m_shader;
	RenderCamera			m_camera;

	/************************************************************************/
	/* message                                                              */
	/************************************************************************/
	bool					m_visible;

// 	/************************************************************************/
// 	/* extend                                                               */
// 	/************************************************************************/
// 	int						m_flags;
//	void*					m_ud;

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_