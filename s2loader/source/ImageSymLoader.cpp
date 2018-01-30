#include "s2loader/ImageSymLoader.h"

#include <gum/Image.h>
#include <gum/ImageSymbol.h>
#include <gum/ImagePool.h>

#include <simp/NodeID.h>

namespace s2loader
{

ImageSymLoader::ImageSymLoader(gum::ImageSymbol& sym)
	: m_sym(sym)
{
}

void ImageSymLoader::Load(const bimp::FilePath& res_path, float scale, bool async)
{
	int pkg_id = simp::NodeID::GetPkgID(m_sym.GetID());
	auto img = gum::ImagePool::Instance()->Create(pkg_id, res_path, async);
	if (!img) {
		return;
	}
	m_sym.SetImage(img);
	sm::ivec2 sz = img->GetSize();
	m_sym.SetRegion(sm::ivec2(0, 0), sz, sm::vec2(0, 0), 0, scale);
}

}