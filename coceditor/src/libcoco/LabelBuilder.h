#ifndef _LIBCOCO_LABEL_BUILDER_H_
#define _LIBCOCO_LABEL_BUILDER_H_

#include "INodeBuilder.h"

#include <drag2d.h>

namespace libcoco
{

class IPackNode;
class PackLabel;

class LabelBuilder : public INodeBuilder
{
public:
	LabelBuilder();
	virtual ~LabelBuilder();

	virtual void ToString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const d2d::FontSprite* spr);

private:
	std::vector<const PackLabel*> m_labels;

}; // LabelBuilder

}

#endif // _LIBCOCO_LABEL_BUILDER_H_