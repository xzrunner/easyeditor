#ifndef _EASYEDITOR_SPRITE_H_
#define _EASYEDITOR_SPRITE_H_

#include "Cloneable.h"
#include "UserDataImpl.h"
#include "Object.h"
#include "Vector.h"
#include "ColorTrans.h"
#include "ShaderTrans.h"

#include <json/json.h>

namespace ee
{

class Vector;
class Rect;
class Symbol;
class BoundingBox;
class Matrix;
class PropertySetting;
class EditPanelImpl;
class SpriteObserver;

class Sprite : public Cloneable, public UserDataImpl, public Object
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	virtual ~Sprite();

	//
	// Cloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr);

	virtual bool Update(int version) = 0;

	virtual const Symbol& GetSymbol() const = 0;
	virtual void SetSymbol(Symbol* symbol) = 0;

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual void BuildBounding();

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	virtual void SetTransform(const Vector& position, float angle);
	const Vector& GetPosition() const { return m_pos; }
	float GetAngle() const { return m_angle; }

	const Vector& GetScale() const { return m_scale; }
	void SetScale(const Vector& scale);

	const Vector& GetShear() const { return m_shear; }
	void SetShear(const Vector& shear);

	const Vector& GetOffset() const { return m_offset; }
	void SetOffset(const Vector& offset);

	bool IsContain(const Vector& pos) const;
	bool IsIntersect(const Rect& rect) const;

	void Translate(const Vector& offset);
	void Rotate(float delta);

	void SetMirror(bool xMirror, bool yMirror);
	void GetMirror(bool& xMirror, bool& yMirror) const { xMirror = m_xMirror; yMirror = m_yMirror; }
	bool GetMirrorX() const { return m_xMirror; }
	bool GetMirrorY() const { return m_yMirror; }

	const Vector& GetPerspective() const { return m_perspective; }
	void SetPerspective(const Vector& perspective) { m_perspective = perspective; }

	Vector GetCenter() const;

	BoundingBox* GetBounding() const {
		return m_bounding;
	}
	Rect GetRect() const;

	void SetObserver(SpriteObserver* observer) {
		m_observer = observer;
	}

	void GetTransMatrix(Matrix& mt) const;
	Matrix GetTransInvMatrix() const;

	bool IsAnchor() const { return m_is_anchor; }
	void SetAnchor(bool anchor) { m_is_anchor = anchor; }

protected:
	template<typename T>
	void SetSymbol(T** dst, Symbol* src);

public:
	// info
	std::string name;
	std::string tag;
	bool clip;

	ColorTrans color;
	ShaderTrans shader;

	// edit
	bool visiable;
	bool editable;

protected:
	Vector m_pos;
	float m_angle;
	Vector m_offset;
	Vector m_scale;
	Vector m_shear;

	bool m_xMirror, m_yMirror;

	Vector m_perspective;

	BoundingBox* m_bounding;
	
	SpriteObserver* m_observer;

private:
	bool m_is_anchor;

}; // Sprite

template<typename T>
void Sprite::SetSymbol(T** dst, Symbol* src)
{
	if (src == *dst) {
		return;
	}
	T* sym = dynamic_cast<T*>(src);
	if (!sym) {
		return;
	}
	obj_assign<T>(*dst, sym);
	BuildBounding();
}

class SpriteCmp
{
public:
	enum Type
	{
		e_file,
		e_x,
		e_y,
		e_x_invert,
		e_y_invert
	};

public:
	SpriteCmp(Type type = e_file);

	bool operator() (const Sprite* s0, const Sprite* s1) const;

private:
	Type m_type;

}; // SpriteCmp

}

#endif // _EASYEDITOR_SPRITE_H_