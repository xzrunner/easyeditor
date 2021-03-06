#ifndef _EASYSPRPACKER_PACK_AUDIO_ID_MGR_H_
#define _EASYSPRPACKER_PACK_AUDIO_ID_MGR_H_

#include <cu/cu_macro.h>

#include <map>
#include <string>

namespace esprpacker
{

class PackAudioIDMgr
{
public:
	void Init(const std::string& filepath);

	int Query(const std::string& filepath) const;

private:
	std::map<std::string, int> m_map2id;

	CU_SINGLETON_DECLARATION(PackAudioIDMgr)

}; // PackAudioIDMgr

}

#endif // _EASYSPRPACKER_PACK_AUDIO_ID_MGR_H_