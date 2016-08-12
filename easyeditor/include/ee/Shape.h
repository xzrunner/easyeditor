#ifndef _EASYEDITOR_SHAPE_H_
#define _EASYEDITOR_SHAPE_H_

#include "UserDataImpl.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <sprite2/RenderColor.h>
#include <sprite2/Shape.h>

#include <json/json.h>

namespace ee
{

class PropertySetting;
class EditPanelImpl;

class Shape : public virtual s2::Shape, public UserDataImpl
{
public:
	Shape();
	Shape(const Shape& shape);
	virtual ~Shape();

	/**
	 *  @interface
	 *    UserDataImpl
	 */
	virtual void ClearUserData(bool deletePtr) {}

	virtual Shape* EEClone() const { return NULL; }

	virtual const char* GetShapeDesc() const = 0;

	virtual void Translate(const sm::vec2& offset) = 0;
//	virtual void Rotate(float delta) = 0;

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage) = 0;

	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	virtual void ReloadTexture() {}

	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

protected:
	static const int QUERY_ACCURACY = 5;

private:
	std::string m_name;

}; // Shape

}

#endif // _EASYEDITOR_SHAPE_H_