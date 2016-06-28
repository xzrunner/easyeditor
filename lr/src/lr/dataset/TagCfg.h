#ifndef _LR_TAG_CFG_H_
#define _LR_TAG_CFG_H_

#include <vector>
#include <string>
#include <map>

namespace ee { class Symbol; }

namespace lr
{

class TagCfg
{
public:
	std::string Query(const ee::Symbol* sym) const;

	static TagCfg* Instance();

private:
	TagCfg();

	void LoadFromFile(const char* filename);

private:
	std::vector<std::pair<std::string, std::string> > m_key_val;

	mutable std::map<const ee::Symbol*, std::string> m_cache;

private:
	static TagCfg* m_instance;

}; // TagCfg

}

#endif // _LR_TAG_CFG_H_