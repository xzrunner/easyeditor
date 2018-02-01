#pragma once

#include "AtomicOP.h"

#include <painting2/Color.h>

namespace ee
{

class SetSpriteColRMapAOP : public AtomicOP
{
public:
	SetSpriteColRMapAOP(const SprPtr& spr, const pt2::Color& color);
	SetSpriteColRMapAOP(const std::vector<SprPtr>& sprs, const pt2::Color& color);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;
	std::vector<pt2::Color> m_old_color;

	pt2::Color m_new_color;

}; // SetSpriteColRMapAOP

}