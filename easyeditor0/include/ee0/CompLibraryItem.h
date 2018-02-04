#pragma once

#include "ee0/WxLibraryItem.h"

#include <sprite2/Component.h>

namespace ee0
{

class CompLibraryItem : public s2::Component
{
public:
//	CompLibraryItem();
	
	virtual CompLibraryItem* Clone() const;

private:
	WxLibraryItem m_item;

}; // CompLibraryItem

}