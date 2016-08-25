#include "Material.h"

namespace eshape
{

void Material::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_tris[i] += offset;
	}
}

}