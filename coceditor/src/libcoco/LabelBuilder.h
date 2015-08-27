#ifndef _LIBCOCO_LABEL_BUILDER_H_
#define _LIBCOCO_LABEL_BUILDER_H_

#include <drag2d.h>

namespace libcoco
{

class IPackNode;
class PackLabel;

class LabelBuilder
{
public:
	LabelBuilder();
	~LabelBuilder();

	const IPackNode* Create(const d2d::FontSprite* spr);

private:
	std::vector<const PackLabel*> m_labels;

}; // LabelBuilder

}

#endif // _LIBCOCO_LABEL_BUILDER_H_