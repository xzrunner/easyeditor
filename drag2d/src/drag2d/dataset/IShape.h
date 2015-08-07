#ifndef _D2D_ISHAPE_H_
#define _D2D_ISHAPE_H_

#include "interfaces.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Color.h"

#include <json/json.h>

namespace d2d
{

class Vector;
class Rect;
class IPropertySetting;
class Matrix;
class EditPanelImpl;

class IShape : public ICloneable, public UserDataImpl, public Object
{
public:
	IShape();
	virtual ~IShape() { ClearUserData(true); }

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr) {}

	//
	// ICloneable interface
	//
	virtual IShape* Clone() const { return NULL; }

	virtual const char* GetShapeDesc() const = 0;

	virtual bool isContain(const Vector& pos) const = 0;
	virtual bool isIntersect(const Rect& rect) const = 0;

	virtual void Translate(const Vector& offset) = 0;
//	virtual void Rotate(float delta) = 0;

	virtual const Rect& getRect() const = 0;

	virtual void draw(const Matrix& mt, 
		const Colorf& color = Colorf(0, 0, 0)) const = 0;

	virtual IPropertySetting* createPropertySetting(EditPanelImpl* stage) = 0;

	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	virtual void ReloadTexture() {}

protected:
	static const int QUERY_ACCURACY = 5;

public:
	std::string name;

}; // IShape

}

#endif // _D2D_ISHAPE_H_