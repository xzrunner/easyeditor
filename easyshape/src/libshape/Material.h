#ifndef _EASYSHAPE_INTERFACE_MATERIAL_H_
#define _EASYSHAPE_INTERFACE_MATERIAL_H_

#include <sprite2/Polygon.h>

#include <json/json.h>

namespace eshape
{

class Material : public virtual s2::Polygon
{
public:
	virtual Json::Value Store(const std::string& dirpath) const = 0;
	virtual void Translate(const sm::vec2& offset);

}; // Material

}

#endif // _EASYSHAPE_INTERFACE_MATERIAL_H_