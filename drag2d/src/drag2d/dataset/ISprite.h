#ifndef _DRAG2D_ISPRITE_H_
#define _DRAG2D_ISPRITE_H_

#include "interfaces.h"
#include "ColorTrans.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Vector.h"
#include "render/BlendModes.h"
#include "render/FilterModes.h"

#include <json/json.h>

namespace d2d
{

class Vector;
class Rect;
class ISymbol;
class AbstractBV;
class Matrix;
class IPropertySetting;
class EditPanelImpl;

class ISprite : public ICloneable, public UserDataImpl, public Object
{
public:
	ISprite();
	ISprite(const ISprite& sprite);
	virtual ~ISprite();

	//
	// ICloneable interface
	//
	virtual ISprite* Clone() const;

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr);

	virtual bool Update(int version) = 0;

	virtual const ISymbol& GetSymbol() const = 0;
	virtual void SetSymbol(ISymbol* symbol) = 0;

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual void BuildBounding();

	virtual IPropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	virtual void SetTransform(const Vector& position, float angle);
	void SetScale(float xScale, float yScale);
	void SetShear(float xShear, float yShear);

	bool IsContain(const Vector& pos) const;
	bool IsIntersect(const Rect& rect) const;

	void Translate(const Vector& offset);
	void Rotate(float delta);

	const Vector& GetPosition() const { return m_pos; }
	const Vector& GetOffset() const { return m_offset; }
	void SetOffset(const Vector& offset);
	float GetAngle() const { return m_angle; }
	const Vector& GetScale() const { return m_scale; }
	const Vector& GetShear() const { return m_shear; }

	void SetMirror(bool xMirror, bool yMirror);
	void GetMirror(bool& xMirror, bool& yMirror) const { xMirror = m_xMirror; yMirror = m_yMirror; }
	bool GetMirrorX() const { return m_xMirror; }
	bool GetMirrorY() const { return m_yMirror; }

	const Vector& GetPerspective() const { return m_perspective; }
	void SetPerspective(const Vector& perspective) { m_perspective = perspective; }

	Vector GetCenter() const;

	AbstractBV* GetBounding() const {
		return m_bounding;
	}
	d2d::Rect GetRect() const;

	BlendMode GetBlendMode() const { return m_blend_mode; }
	void SetBlendMode(BlendMode mode) { m_blend_mode = mode; }

	FilterMode GetFilterMode() const { return m_filter_mode; } 
	void SetFilterMode(FilterMode mode) { m_filter_mode = mode; }

	void setObserver(ISpriteObserver* observer) {
		m_observer = observer;
	}

	void GetTransMatrix(Matrix& mt) const;
	Matrix GetTransInvMatrix() const;

	bool IsAnchor() const { return m_is_anchor; }
	void SetAnchor(bool anchor) { m_is_anchor = anchor; }

protected:
	template<typename T>
	void SetSymbol(T** dst, d2d::ISymbol* src);

public:
	// info
	std::string name;
	ColorTrans color;
	std::string tag;
	bool clip;

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

	BlendMode m_blend_mode;
	FilterMode m_filter_mode;

	AbstractBV* m_bounding;

	ISpriteObserver* m_observer;

private:
	bool m_is_anchor;

}; // ISprite

template<typename T>
void ISprite::SetSymbol(T** dst, d2d::ISymbol* src)
{
	if (src == *dst) {
		return;
	}
	T* sym = dynamic_cast<T*>(src);
	if (!sym) {
		return;
	}
	d2d::obj_assign<T>(*dst, sym);
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

	bool operator() (const ISprite* s0, const ISprite* s1) const;

private:
	Type m_type;

}; // SpriteCmp

}

#endif // _DRAG2D_ISPRITE_H_