#ifndef _LIBCOCO_LABEL_PACKER_H_
#define _LIBCOCO_LABEL_PACKER_H_

#include "INodePacker.h"

#include <drag2d.h>

namespace libcoco
{

class IPackNode;
class PackLabel;

class LabelPacker : public INodePacker
{
public:
	LabelPacker();
	virtual ~LabelPacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const d2d::FontSprite* spr);

private:
	std::vector<const PackLabel*> m_labels;

}; // LabelPacker

}

#endif // _LIBCOCO_LABEL_PACKER_H_
