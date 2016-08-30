#ifndef _EASYSPRPACKER_PACK_NODE_FACTORY_H_
#define _EASYSPRPACKER_PACK_NODE_FACTORY_H_

#include <CU_Singleton.h>

#include <string>

namespace ee { class Sprite; class Symbol; }

namespace esprpacker
{

class PackNode;

class PackNodeFactory
{
public:
	const PackNode* Create(const ee::Sprite* spr);
	const PackNode* Create(const ee::Symbol* sym);

	void SetFilesDir(const std::string& files_dir) {
		m_files_dir = files_dir;
	}

private:
	std::string m_files_dir;

	SINGLETON_DECLARATION(PackNodeFactory);

}; // PackNodeFactory

}

#endif // _EASYSPRPACKER_PACK_NODE_FACTORY_H_