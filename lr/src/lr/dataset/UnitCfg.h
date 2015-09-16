#ifndef _LR_UNIT_CFG_H_
#define _LR_UNIT_CFG_H_

#include <vector>

namespace lr
{

class UnitWidget;

class UnitCfg
{
public:
	static UnitCfg* Instance();

private:
	UnitCfg() {}

	void LoadFromFile(const char* filename);

public:
	std::vector<UnitWidget*> widgets;

private:
	static bool m_loaded;

	static UnitCfg* m_instance;

}; // UnitCfg

}

#endif // _LR_UNIT_CFG_H_