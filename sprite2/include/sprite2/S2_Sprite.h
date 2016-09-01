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

#include <string>

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
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);
	virtual ~Sprite();

	virtual void Translate(const sm::vec2& offset);
	virtual void Rotate(float delta);
	
 	virtual bool Update(const RenderParams& params) { return false; }
	virtual Sprite* FetchChild(const std::string& name) const { return NULL; }

	virtual void SetSymbol(Symbol* sym);

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Sprite* Clone() const { return NULL; }

	Symbol* GetSymbol() { return m_sym; }
	const Symbol* GetSymbol() const { return m_sym; }

	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

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

	bool IsVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }

	sm::mat4 GetTransMatrix() const;
	sm::mat4 GetTransInvMatrix() const;

protected:
	Symbol*					m_sym;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	std::string				m_name;

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