#ifndef _EASYANIM_LOVE2D_CODE_H_
#define _EASYANIM_LOVE2D_CODE_H_

#include <easybuilder.h>

namespace eanim
{

class LayersMgr;

class Love2dCode
{
public:
	Love2dCode(ebuilder::CodeGenerator& gen, LayersMgr* layers);

	void Resolve();

private:
	int Query(const std::string& filepath) const;

private:
	ebuilder::CodeGenerator& m_gen;

	LayersMgr* m_layers;

	d2d::TexPackerAdapter m_tp_adapter;

}; // Love2dCode

}

#endif // _EASYANIM_LOVE2D_CODE_H_