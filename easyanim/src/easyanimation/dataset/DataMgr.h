#ifndef _EASYANIM_DATA_MGR_H_
#define _EASYANIM_DATA_MGR_H_

#include "LayersMgr.h"
#include "AnimTemplate.h"

namespace eanim
{

class DataMgr
{
public:
	const LayersMgr& GetLayers() const { return m_layers; }
	LayersMgr& GetLayers() { return m_layers; }

	const AnimTemplate& GetTemplate() const { return m_anim_template; }
	AnimTemplate& GetTemplate() { return m_anim_template; }

	static DataMgr* Instance();

private:
	DataMgr() {}

public:
	std::string name;
	std::string tag;

private:
	LayersMgr m_layers;

	AnimTemplate m_anim_template;

private:
	static DataMgr* m_instance;

}; // DataMgr

}

#endif // _EASYANIM_DATA_MGR_H_