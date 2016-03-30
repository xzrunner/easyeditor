#ifndef _EASYSHAPE_COLOR_MATERIAL_H_
#define _EASYSHAPE_COLOR_MATERIAL_H_

#include "Material.h"

#include <ee/Vector.h>
#include <ee/Color.h>

namespace ee { class Matrix; }

namespace eshape
{

class ColorMaterial : public Material
{
public:
	ColorMaterial(const std::vector<ee::Vector>& vertices,
		const ee::Colorf& color);

	virtual Json::Value Store(const std::string& dirpath) const;

	virtual void Draw(const ee::Matrix& mt, const ee::RenderColor& color) const;

	virtual void ReloadTexture() {}

	const ee::Colorf& GetColor() const { return m_color; }

protected:
	virtual void BuildEnd();

private:
	ee::Colorf m_color;

}; // ColorMaterial

}

#endif // _EASYSHAPE_COLOR_MATERIAL_H_