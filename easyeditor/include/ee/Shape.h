#ifndef _EASYEDITOR_SHAPE_H_
#define _EASYEDITOR_SHAPE_H_

#include "Cloneable.h"
#include "UserDataImpl.h"
#include "Object.h"
#include "RenderColor.h"

#include <json/json.h>

namespace ee
{

class Vector;
class Rect;
class PropertySetting;
class Matrix;
class EditPanelImpl;
class RenderColor;

class Shape : public Cloneable, public UserDataImpl, public Object
{
public:
	Shape();
	virtual ~Shape() { ClearUserData(true); }

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr) {}

	//
	// Cloneable interface
	//
	virtual Shape* Clone() const { return NULL; }

	virtual const char* GetShapeDesc() const = 0;

	virtual bool IsContain(const Vector& pos) const = 0;
	virtual bool IsIntersect(const Rect& rect) const = 0;

	virtual void Translate(const Vector& offset) = 0;
//	virtual void Rotate(float delta) = 0;

	virtual const Rect& GetRect() const = 0;

	virtual void Draw(const Matrix& mt, 
		const RenderColor& color = RenderColor()) const = 0;

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage) = 0;

	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	virtual void ReloadTexture() {}

protected:
	static const int QUERY_ACCURACY = 5;

public:
	std::string name;

}; // Shape

}

#endif // _EASYEDITOR_SHAPE_H_