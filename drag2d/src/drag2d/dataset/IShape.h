#ifndef _D2D_ISHAPE_H_
#define _D2D_ISHAPE_H_

#include "interfaces.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Color.h"

namespace d2d
{

class Vector;
class Rect;
class IPropertySetting;

class IShape : public ICloneable, public UserDataImpl, public Object
{
public:
	IShape();
	virtual ~IShape() { clearUserData(true); }

	//
	// UserDataImpl interface
	//	
	virtual void clearUserData(bool deletePtr) {}

	//
	// ICloneable interface
	//
	virtual IShape* clone() const { return NULL; }

	virtual bool isContain(const Vector& pos) const = 0;
	virtual bool isIntersect(const Rect& rect) const = 0;

	virtual void Translate(const Vector& offset) = 0;
//	virtual void Rotate(float delta) = 0;

	virtual const Rect& getRect() const = 0;

	virtual void draw(const d2d::Matrix& mt, 
		const Colorf& color = Colorf(0, 0, 0)) const = 0;

	virtual IPropertySetting* createPropertySetting(EditPanel* editPanel) = 0;

protected:
	static const int QUERY_ACCURACY = 5;

public:
	std::string name;

}; // IShape

}

#endif // _D2D_ISHAPE_H_