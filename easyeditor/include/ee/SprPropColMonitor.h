#ifndef _EASYEDITOR_SPR_PROP_COL_MONITOR_H_
#define _EASYEDITOR_SPR_PROP_COL_MONITOR_H_

#include "ColorMonitor.h"
#include "Sprite.h"

namespace ee
{

class SprPropColMonitor : public ColorMonitor
{
public:
	enum ColType
	{
		CT_MUL = 0,
		CT_ADD,
		CT_RMAP,
		CT_GMAP,
		CT_BMAP,
	};

public:
	SprPropColMonitor(const SprPtr& spr, ColType type);

	//
	// interface ColorMonitor
	//
	virtual s2::Color GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const s2::Color& col);

private:
	SprPtr m_spr;
	ColType m_type;

}; // SprPropColMonitor

}

#endif // _EASYEDITOR_SPR_PROP_COL_MONITOR_H_