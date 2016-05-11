#ifndef _EASYSHAPE_COLOR_MATERIAL_H_
#define _EASYSHAPE_COLOR_MATERIAL_H_

#include "Material.h"

#include <SM_Matrix.h>
#include <sprite2/Color.h>

namespace eshape
{

class ColorMaterial : public Material
{
public:
	ColorMaterial(const std::vector<sm::vec2>& vertices,
		const s2::Color& color);

	virtual Json::Value Store(const std::string& dirpath) const;

	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color) const;

	virtual void ReloadTexture() {}

	const s2::Color& GetColor() const { return m_color; }

protected:
	virtual void BuildEnd();

private:
	s2::Color m_color;

}; // ColorMaterial

}

#endif // _EASYSHAPE_COLOR_MATERIAL_H_